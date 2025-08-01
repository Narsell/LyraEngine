#include "lrpch.h"

#include "PerspectiveCameraController.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseButtonCodes.h"

namespace Lyra
{
	PerspectiveCameraController::PerspectiveCameraController()
		:	m_Window(Application::Get().GetWindow()),
			m_CameraInitialPos(0.0, 0.0f, 2.0f),
			m_CameraMinSpeed(2.0f),
			m_CameraMaxSpeed(60.0f),
			m_CameraSpeed(10.0f),
			m_ZoomSpeedFactor(0.005f),
			m_LookAtSensitivity(0.1f),
			m_MouseLastX(m_Window.GetWidth() / 2.0f),
			m_MouseLastY(m_Window.GetHeight() / 2.0f)
	{
	}

	void PerspectiveCameraController::AttachToCamera(const Ref<PerspectiveCamera>& camera)
	{
		if (m_Camera.get())
		{
			LR_CORE_WARN("Camera controller tried to attach to camera but it is already attached to one.");
			return;
		}
		m_Camera = camera;
		m_LookAtSensitivity = m_ZoomSpeedFactor * m_Camera->GetFOV();
		m_Camera->SetPosition(m_CameraInitialPos);
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		if (!m_Camera.get()) return;

		glm::vec3 direction = { 0.0f, 0.0f, 0.0f };

		if (Input::IsKeyPressed(LR_KEY_W))
		{
			direction += m_Camera->GetForward();
		}
		else if (Input::IsKeyPressed(LR_KEY_S))
		{
			direction += -m_Camera->GetForward();
		}
		if (Input::IsKeyPressed(LR_KEY_D))
		{
			direction += m_Camera->GetRight();
		}
		else if (Input::IsKeyPressed(LR_KEY_A))
		{
			direction += -m_Camera->GetRight();
		}
		if (Input::IsKeyPressed(LR_KEY_SPACE))
		{
			direction += m_Camera->GetUp();
		}
		else if (Input::IsKeyPressed(LR_KEY_LEFT_CONTROL))
		{
			direction += -m_Camera->GetUp();
		}

		if (glm::length(direction) > 0.0f)
		{
			glm::vec3 newPosition = m_Camera->GetPosition() + (glm::normalize(direction) * m_CameraSpeed * ts.GetSeconds());
			m_Camera->SetPosition(newPosition);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		if (!m_Camera.get()) return;

		m_Camera->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(LR_BIND_EVENT_FN(&PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(LR_BIND_EVENT_FN(&PerspectiveCameraController::OnMouseScrolled));
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!m_Camera.get()) return false;

		float mouseX = static_cast<float>(e.GetMouseX());
		float mouseY = static_cast<float>(e.GetMouseY());

		if (m_FirstMouseMovement)
		{
			m_MouseLastX = mouseX;
			m_MouseLastY = mouseY;
			m_FirstMouseMovement = false;
		}

		float xOffset = mouseX - m_MouseLastX;
		float yOffset = m_MouseLastY - mouseY;

		m_MouseLastX = mouseX;
		m_MouseLastY = mouseY;

		xOffset *= m_LookAtSensitivity;
		yOffset *= m_LookAtSensitivity;

		m_Camera->SetRotationOffset(xOffset, yOffset, 0.0f);

		return false;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (!m_Camera.get()) return false;

		if (Input::IsKeyPressed(LR_KEY_LEFT_SHIFT))
		{
			float currentFOV = m_Camera->GetFOV();
			m_Camera->SetFOV(currentFOV - (float)e.GetYOffset());
			m_LookAtSensitivity = m_ZoomSpeedFactor * currentFOV;
			//LR_CORE_INFO("Sensitivity: {0} | FOV: {1}", m_LookAtSensitivity, currentFOV);
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