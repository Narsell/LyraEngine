#include "lrpch.h"

#include "Camera.h"

namespace Lyra
{
	Camera::Camera(glm::mat4 projectionMatrix, glm::vec3 position)
		:	m_ProjectionMatrix(projectionMatrix),
			m_Position(position),
			m_ViewMatrix(glm::translate(glm::mat4(1.0f), m_Position)),
			m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
	}
	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

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
		: Camera(glm::perspective(fov, aspectRatio, zNear, zFar), { 0.0f, 0.0, 0.0f })
	{
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(transform, m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
