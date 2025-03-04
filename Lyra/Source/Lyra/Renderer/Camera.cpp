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

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f), { 0.0f, 0.0f, 0.0f })
	{
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// TODO: Optimize this so it only calculates what changed.
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(transform, m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/* * * * * * * * * * * * * 
	 *   PERSPECTIVE CAMERA  *
	 * * * * * * * * * * * * */

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar)
		: Camera(glm::perspective(fov, aspectRatio, zNear, zFar), { 0.0f, 0.0, 0.0f }),
			m_Forward(glm::vec3(0.0f)),
			m_Right(glm::vec3(0.0f)),
			m_Up(glm::vec3(0.0f))
	{
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::ProcessMouseMovement(float xOffset, float yOffset)
	{
		xOffset *= m_LookAtSensitivity;
		yOffset *= m_LookAtSensitivity;

		m_Yaw += xOffset;
		m_Pitch += yOffset;
		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		// Look up Gram–Schmidt process!
		m_Forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward.y = sin(glm::radians(m_Pitch));
		m_Forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward	= glm::normalize(m_Forward);

		m_Right		= glm::normalize(glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up		= glm::normalize(glm::cross(m_Right, m_Forward));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
