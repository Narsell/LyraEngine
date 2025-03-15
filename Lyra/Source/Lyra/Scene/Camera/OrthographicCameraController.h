#pragma once

#include "Core/Window.h"
#include "Core/Timestep.h"
#include "Events/MouseEvent.h"
#include "Scene/Camera/Camera.h"


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