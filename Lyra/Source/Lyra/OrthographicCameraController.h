#pragma once

#include "Lyra/Core/Window.h"
#include "Lyra/Core/Timestep.h"
#include "Lyra/Renderer/Camera.h"
#include "Lyra/Events/ApplicationEvent.h"
#include "Lyra/Events/MouseEvent.h"


namespace Lyra
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(bool rotation = false);

		void OnUpdate(Timestep ts);

		void OnEvent(Event& e);

		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		Window& m_Window;

		OrthographicCamera m_Camera;
		
		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		float m_ZoomSpeedFactor = 1.3f;
		float m_CameraMoveSpeed = 3.0f;
		float m_CameraRotationSpeed = 20.f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 2.0f };
	};
}