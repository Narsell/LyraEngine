#pragma once

#include "Lyra/Core/Timestep.h"
#include "Lyra/Core/Window.h"
#include "Lyra/Events/Event.h"
#include "Lyra/Events/MouseEvent.h"
#include "Lyra/Renderer/Camera.h"

namespace Lyra
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController();

		void OnUpdate(Timestep ts);

		void OnEvent(Event& e);

		const PerspectiveCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

	private:
		Window& m_Window;

		PerspectiveCamera m_Camera;
		glm::vec3 m_CameraInitialPos;
		float m_CameraMinSpeed, m_CameraMaxSpeed, m_CameraSpeed;
		float m_ZoomSpeedFactor;
		float m_LookAtSensitivity;

		bool m_FirstMouseMovement = true;
		float m_MouseLastX, m_MouseLastY;
	};

}
