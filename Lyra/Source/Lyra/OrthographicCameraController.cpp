#include "lrpch.h"

#include "OrthographicCameraController.h"

#include "Lyra/Input/Input.h"
#include "Lyra/Input/KeyCodes.h"

namespace Lyra
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:	m_AspectRatio(aspectRatio),
			m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
			m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		// Offsets
		glm::vec3 offsetPosition = {0.0f, 0.0f, 0.0f};
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

		// Get rotation input
		if (m_Rotation)
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
		if (offsetPosition != glm::vec3(0.0f))
		{
			offsetPosition = glm::normalize(offsetPosition) * (m_CameraMoveSpeed * ts);
			m_CameraPosition = m_Camera.GetPosition() + offsetPosition;
			m_Camera.SetPosition(m_CameraPosition);
		}

		if (m_Rotation && offsetRotation != 0.0f)
		{
			m_CameraRotation = m_Camera.GetRotation() + offsetRotation;
			m_Camera.SetRotation(m_CameraRotation);
		}

		// This maps our movement speed to the zoom level (More zoomed in = less speed, less zoomed in = more speed)
		m_CameraMoveSpeed = 1.3 * m_ZoomLevel;

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(LR_BIND_EVENT_FN(&OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(LR_BIND_EVENT_FN(&OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel = std::clamp(m_ZoomLevel - ((float)e.GetYOffset() / 5.0f), m_MinZoom, m_MaxZoom);
		LR_CORE_INFO("{0}", m_ZoomLevel);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}