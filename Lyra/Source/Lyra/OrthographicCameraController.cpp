#include "lrpch.h"

#include "OrthographicCameraController.h"

#include "Lyra/Core/Application.h"
#include "Lyra/Input/Input.h"
#include "Lyra/Input/KeyCodes.h"

namespace Lyra
{
	OrthographicCameraController::OrthographicCameraController(bool rotation)
		:	m_Window(Application::GetApplication().GetWindow()),
			m_Camera(m_Window.GetAspectRatio()),
			m_Rotation(rotation)
	{
		m_Camera.SetPosition(m_CameraPosition);
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
		m_CameraMoveSpeed = m_ZoomSpeedFactor * m_Camera.GetZoomLevel();

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);
	}

}