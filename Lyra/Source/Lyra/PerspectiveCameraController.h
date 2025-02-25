#pragma once
#include "Lyra/Renderer/Camera.h"
#include "Lyra/Core/Timestep.h"
#include "Lyra/Events/Event.h"

namespace Lyra
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float nearDist, float farDist);
		PerspectiveCameraController();

		void OnUpdate(Timestep ts);

		void OnEvent(Event& e);

		const PerspectiveCamera& GetCamera() const { return m_Camera; }
	private:
		float m_FOV = 45.0f;
		float m_AspectRatio = 16.f / 9.f;
		float m_Near = 0.1f;
		float m_Far = 100.0f;

		PerspectiveCamera m_Camera;

		float m_CameraMoveSpeed = 3.f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 5.0f };

	};

}
