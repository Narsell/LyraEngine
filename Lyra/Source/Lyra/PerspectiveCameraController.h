#pragma once

#include "Lyra/Core/Timestep.h"
#include "Lyra/Core/Window.h"
#include "Lyra/Renderer/Camera.h"
#include "Lyra/Events/Event.h"
#include "Lyra/Events/MouseEvent.h"

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

	private:
		Window& m_Window;

		float m_CameraMoveSpeed = 6.f;
		glm::vec3 m_CameraPosition = { 0.0, 0.0f, 2.0f };
		PerspectiveCamera m_Camera;

		float m_MouseLastX, m_MouseLastY;
		bool m_FirstMouseMovement = true;
	};

}
