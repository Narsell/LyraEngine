#pragma once

#include "Lyra/Renderer/Camera.h"
#include "Lyra/Core/Timestep.h"
#include "Lyra/Events/ApplicationEvent.h"
#include "Lyra/Events/MouseEvent.h"

namespace Lyra
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);

		void OnEvent(Event& e);

		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio = 16.f / 9.f;
		float m_ZoomLevel = 1.0f;
		float m_MaxZoom = 20.0f;
		float m_MinZoom = 0.20f;
		OrthographicCamera m_Camera;
		
		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 3.f;
		float m_CameraRotationSpeed = 20.f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	};
}