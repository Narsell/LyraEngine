#include "lrpch.h"

#include "PerspectiveCameraController.h"

#include "Lyra/Core/Application.h"
#include "Lyra/Input/Input.h"
#include "Lyra/Input/KeyCodes.h"
#include "Lyra/Input/MouseButtonCodes.h"

namespace Lyra
{
	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar)
		:	m_FOV(fov),
			m_AspectRatio(aspectRatio),
			m_Near(zNear),
			m_Far(zFar),
			m_Camera(m_FOV, m_AspectRatio, m_Near, m_Far),
			m_Window(Application::GetApplication().GetWindow()),
			m_MouseLastX(m_Window.GetWidth() / 2.0f),
			m_MouseLastY(m_Window.GetHeight() / 2.0f),
			m_Time(0.0f)
	{
		m_Camera.SetPosition(m_CameraPosition);
	}

	PerspectiveCameraController::PerspectiveCameraController()
		: PerspectiveCameraController(m_FOV, m_AspectRatio, m_Near, m_Far) {}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		m_Time += ts.GetSeconds();

		glm::vec3 direction = { 0.0f, 0.0f, 0.0f };

		if (Input::IsKeyPressed(LR_KEY_W))
		{
			direction += m_Camera.GetForward();
		}
		else if (Input::IsKeyPressed(LR_KEY_S))
		{
			direction += -m_Camera.GetForward();
		}
		if (Input::IsKeyPressed(LR_KEY_D))
		{
			direction += m_Camera.GetRight();
		}
		else if (Input::IsKeyPressed(LR_KEY_A))
		{
			direction += -m_Camera.GetRight();
		}
		if (Input::IsKeyPressed(LR_KEY_SPACE))
		{
			direction += m_Camera.GetUp();
		}
		else if (Input::IsKeyPressed(LR_KEY_LEFT_CONTROL))
		{
			direction += -m_Camera.GetUp();
		}

		if (glm::length(direction) > 0.0f)
		{
			m_CameraPosition += (glm::normalize(direction) * m_CameraMoveSpeed * ts.GetSeconds());
			m_Camera.SetPosition(m_CameraPosition);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMovedEvent>(LR_BIND_EVENT_FN(&PerspectiveCameraController::OnMouseMoved));
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		float mouseX = e.GetMouseX();
		float mouseY = e.GetMouseY();

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

		m_Camera.ProcessMouseMovement(xOffset, yOffset);

		return false;
	}
}