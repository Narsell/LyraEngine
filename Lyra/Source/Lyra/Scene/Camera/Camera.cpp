#include "lrpch.h"

#include "Camera.h"

namespace Lyra
{
	/* * * * * * * * * * * * *
	 *       BASE CAMERA     *
	 * * * * * * * * * * * * */

	Camera::Camera(glm::mat4 projectionMatrix, glm::vec3 position)
		:	m_ProjectionMatrix(projectionMatrix),
			m_Position(position),
			m_ViewMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), m_Position))), // This might not be enough, problem for future me.
			m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	/* * * * * * * * * * * * *
	 *   ORTHOGRAPHIC CAMERA *
	 * * * * * * * * * * * * */

	OrthographicCamera::OrthographicCamera(float aspectRatio, float zoomLevel, float zNear, float zFar)
		:	Camera(glm::ortho(-aspectRatio * zoomLevel, aspectRatio* zoomLevel, -zoomLevel, zoomLevel, zNear, zFar), 
				   { 0.0f, 0.0f, 0.0f }),
			m_MinZoomLevel(0.20f),
			m_MaxZoomLevel(20.0f),
			m_AspectRatio(aspectRatio),
			m_ZoomLevel(std::clamp(zoomLevel, m_MinZoomLevel, m_MaxZoomLevel)),
			m_ZNear(zNear),
			m_ZFar(zFar)
	{
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_ShouldRotate = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetZoomLevel(float newZoom)
	{
		m_ZoomLevel = std::clamp(newZoom, m_MinZoomLevel, m_MaxZoomLevel);
		RecalculateProjectionMatrix();
	}

	void OrthographicCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(LR_BIND_EVENT_FN(&OrthographicCamera::OnWindowResized));
	}

	bool OrthographicCamera::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = e.GetAspectRatio();
		RecalculateProjectionMatrix();
		return false;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(transform, m_Position);
		transform = glm::rotate(transform, glm::radians(m_ShouldRotate), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_ZNear, m_ZFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/* * * * * * * * * * * * * 
	 *   PERSPECTIVE CAMERA  *
	 * * * * * * * * * * * * */

	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fov, float zoomLevel, float zNear, float zFar)
		:	Camera(glm::perspective(fov, aspectRatio, zNear, zFar), 
				   { 0.0f, 0.0, 0.0f }),
			m_MinFOV(5.0f),
			m_MaxFOV(fov),
			m_FOV(std::clamp(fov, m_MinFOV, m_MaxFOV)),
			m_AspectRatio(aspectRatio),
			m_ZNear(zNear),
			m_ZFar(zFar),
			m_Forward(glm::vec3(0.0f)),
			m_Right(glm::vec3(0.0f)),
			m_Up(glm::vec3(0.0f))
	{
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetRotationOffset(float xOffset, float yOffset, float zOffset)
	{
		m_Yaw += xOffset;
		m_Pitch += yOffset;
		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
		RecalculateViewMatrix();
		if (zOffset != 0.0f)
		{
			LR_CORE_WARN("PerspectiveCamera does not support roll rotation (yet), but a value of {0} was given.", zOffset);
		}
	}

	void PerspectiveCamera::SetRotationOffset(const glm::vec3& rotationOffset)
	{
		SetRotationOffset(rotationOffset.x, rotationOffset.y, rotationOffset.z);
	}

	void PerspectiveCamera::SetFOV(float newFOV)
	{
		m_FOV = std::clamp(newFOV, m_MinFOV, m_MaxFOV);
		RecalculateProjectionMatrix();
	}

    void PerspectiveCamera::SetRenderDistance(float newZFar)
    {
		m_ZFar = newZFar;
		RecalculateProjectionMatrix();
    }

    void PerspectiveCamera::SetAspectRatio(float newAspectRatio)
	{
		m_AspectRatio = newAspectRatio;
		RecalculateProjectionMatrix();
	}

	void PerspectiveCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(LR_BIND_EVENT_FN(&PerspectiveCamera::OnWindowResized));
	}

	bool PerspectiveCamera::OnWindowResized(WindowResizeEvent& e)
	{
		WindowResizeEvent& re = static_cast<WindowResizeEvent&>(e);
		if (re.GetWidth() > 0 && re.GetHeight() > 0)
		{
			SetAspectRatio(re.GetAspectRatio());
		}
		return false;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		// Look up GramSchmidt process!
		m_Forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward.y = sin(glm::radians(m_Pitch));
		m_Forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward	= glm::normalize(m_Forward);

		m_Right		= glm::normalize(glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up		= glm::normalize(glm::cross(m_Right, m_Forward));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_ZNear, m_ZFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
