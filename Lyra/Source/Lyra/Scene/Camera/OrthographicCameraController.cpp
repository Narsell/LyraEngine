#include "lrpch.h"

#include "OrthographicCameraController.h"

#include "Core/Application.h"
#include "Input/Input.h"
#include "Core/Input/KeyCodes.h"

namespace Lyra
{
	OrthographicCameraController::OrthographicCameraController(bool rotate)
		:	m_Window(Application::Get().GetWindow()),
			m_Camera(m_Window.GetAspectRatio()),
			m_CameraInitialPos(0.0f, 0.0f, 2.0f),
			m_CameraInitialRotation(0.0f),
			m_ShouldRotate(rotate),
			m_ZoomSpeedFactor(1.3f),
			m_CameraMinSpeed(0.5f),
			m_CameraMaxSpeed(20.0f),
			m_CameraSpeed(3.0f),
			m_CameraRotationSpeed(20.0f)
	{
		m_Camera.SetPosition(m_CameraInitialPos);
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		// Offsets
		glm::vec3 direction = {0.0f, 0.0f, 0.0f};
		float offsetRotation = 0.0f;

		// Get movement input
		if (Input::IsKeyPressed(LR_KEY_W))
		{
			direction.y += 1.0f;
		}
		else if (Input::IsKeyPressed(LR_KEY_S))
		{
			direction.y -= 1.0f;
		}
		if (Input::IsKeyPressed(LR_KEY_D))
		{
			direction.x += 1.0f;
		}
		else if (Input::IsKeyPressed(LR_KEY_A))
		{
			direction.x -= 1.0f;
		}

		// Get rotation input
		if (m_ShouldRotate)
		{
			if (Input::IsKeyPressed(LR_KEY_E))
			{
				offsetRotation -= m_CameraRotationSpeed * ts;
				m_Camera.SetRotation(m_Camera.GetRotation() - (m_CameraRotationSpeed * ts));
			}
			else if (Input::IsKeyPressed(LR_KEY_Q))
			{
				offsetRotation += m_CameraRotationSpeed * ts;
				m_Camera.SetRotation(m_Camera.GetRotation() + (m_CameraRotationSpeed * ts));
			}
		}

		// Set offsets if they have been modified by input
		if (glm::length(direction) > 0.0f)
		{
			glm::vec3 newPosition = m_Camera.GetPosition() + glm::normalize(direction) * (m_CameraSpeed * ts);
			m_Camera.SetPosition(newPosition);
		}

		if (m_ShouldRotate && offsetRotation != 0.0f)
		{
			float newRotation = m_Camera.GetRotation() + offsetRotation;
			m_Camera.SetRotation(newRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<MouseScrolledEvent>(LR_BIND_EVENT_FN(&OrthographicCameraController::OnMouseScrolled));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		/* TODO: Adjust speed min and max values based on zoom level and then clamp speed around those. */
		if (Input::IsKeyPressed(LR_KEY_LEFT_SHIFT))
		{
			float currentZoom = m_Camera.GetZoomLevel();
			m_Camera.SetZoomLevel(currentZoom - (static_cast<float>(e.GetYOffset()) / 5.0f));
			LR_CORE_INFO("ZoomLevel: {0}", currentZoom);
		}
		else
		{
			m_CameraSpeed = std::clamp(
				m_CameraSpeed + static_cast<float>(e.GetYOffset()),
				m_CameraMinSpeed,
				m_CameraMaxSpeed
			);
		}
		return false;
	}



}