#pragma once

#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Scene/Camera/Camera.h"

namespace Lyra
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController();
		void AttachToCamera(const Ref<PerspectiveCamera>& camera);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const Ref<PerspectiveCamera>& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

	private:
		Window& m_Window;

		Ref<PerspectiveCamera> m_Camera;
		glm::vec3 m_CameraInitialPos;
		float m_CameraMinSpeed, m_CameraMaxSpeed, m_CameraSpeed;
		float m_ZoomSpeedFactor;
		float m_LookAtSensitivity;

		bool m_FirstMouseMovement = true;
		float m_MouseLastX, m_MouseLastY;
	};

}
