#pragma once

#include <glm/glm.hpp>

namespace Lyra
{

	class Camera
	{

	protected:
		Camera(glm::mat4 projectionMatrix, glm::vec3 position);

	public:
		virtual ~Camera() = default;

		virtual const glm::vec3& GetPosition() const { return m_Position; }
		virtual void SetPosition(const glm::vec3& position);

		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void RecalculateViewMatrix() = 0;
		virtual void RecalculateProjectionMatrix() = 0;

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::vec3 m_Position;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		const float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation);

	private:
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateProjectionMatrix() override;

	private:
		// Since this is an orthographic camera we only need rotation around one axis.
		float m_Rotation = 0.0f;

	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar);

		const glm::vec3 GetRight() const { return m_Right; }
		const glm::vec3 GetUp() const { return m_Up; }
		const glm::vec3 GetForward() const { return m_Forward; }

		void ProcessMouseMovement(float xOffset, float yOffset);

		void SetFOV(float newFOV);
		void SetAspectRatio(float newAspectRatio);

		bool OnEvent(Event& e);

	private:
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateProjectionMatrix() override;

	private:

		float m_FOV, m_AspectRatio, m_ZNear, m_ZFar;

		glm::vec3 m_Forward;
		glm::vec3 m_Right;
		glm::vec3 m_Up;


		float m_LookAtSensitivity = 0.2f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

	};
}
