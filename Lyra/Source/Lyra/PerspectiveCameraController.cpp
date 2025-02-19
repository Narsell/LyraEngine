#include "lrpch.h"

#include "PerspectiveCameraController.h"

#include "Lyra/Input/Input.h"
#include "Lyra/Input/KeyCodes.h"

namespace Lyra
{
	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar)
		:	m_FOV(fov),
			m_AspectRatio(aspectRatio),
			m_Near(zNear),
			m_Far(zFar),
			m_Camera(m_FOV, m_AspectRatio, m_Near, m_Far)
	{
		m_Camera.SetPosition(m_CameraPosition);
	}

	PerspectiveCameraController::PerspectiveCameraController()
		: PerspectiveCameraController(m_FOV, m_AspectRatio, m_Near, m_Far) {}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		// Offsets
		glm::vec3 offsetPosition = { 0.0f, 0.0f, 0.0f };
		float offsetRotation = 0.0f;

		// Get movement input
		if (Input::IsKeyPressed(LR_KEY_W))
		{
			offsetPosition.y += 1.0f;
		}
		else if (Input::IsKeyPressed(LR_KEY_S))
		{
			offsetPosition.y -= 1.0f;
		}
		if (Input::IsKeyPressed(LR_KEY_D))
		{
			offsetPosition.x += 1.0f;
		}
		else if (Input::IsKeyPressed(LR_KEY_A))
		{
			offsetPosition.x -= 1.0f;
		}
		if (Input::IsKeyPressed(LR_KEY_KP_ADD))
		{
			offsetPosition.z -= 1.0f;
		}
		else if (Input::IsKeyPressed(LR_KEY_KP_SUBTRACT))
		{
			offsetPosition.z += 1.0f;
		}

		if (offsetPosition != glm::vec3(0.0f))
		{
			offsetPosition = glm::normalize(offsetPosition) * (m_CameraMoveSpeed * ts);
			m_CameraPosition = m_Camera.GetPosition() + offsetPosition;
			m_Camera.SetPosition(m_CameraPosition);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
	}
}