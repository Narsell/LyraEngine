#pragma once

#include "Lyra/Core/Window.h"
#include "Lyra/Core/Timestep.h"
#include "Lyra/Events/MouseEvent.h"
#include "Lyra/Renderer/Camera.h"


namespace Lyra
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(bool rotate = false);

		void OnUpdate(Timestep ts);

		void OnEvent(Event& e);

		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);

	private:
		Window& m_Window;

		OrthographicCamera m_Camera;
		glm::vec3 m_CameraInitialPos;
		float m_CameraInitialRotation;
		bool m_ShouldRotate;
		float m_ZoomSpeedFactor;
		float m_CameraMinSpeed, m_CameraMaxSpeed, m_CameraSpeed;
		float m_CameraRotationSpeed;
	};
}