#include "lrpch.h"

#include "Camera.h"

namespace Lyra
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:	m_Position(0.0f, 0.0f, 0.0f),
			m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			m_ViewMatrix(glm::mat4(1.0f)),
			m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position; 
		RecalculateViewMatrix();
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
}
