#pragma once

#include <glm/glm.hpp>

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Lyra
{

	class Camera
	{

	protected:
		Camera(glm::mat4 projectionMatrix, glm::vec3 position);

	public:
		virtual ~Camera() = default;

		virtual inline float GetRenderDistance() const = 0;
		virtual inline const glm::vec3& GetPosition() const { return m_Position; }
		virtual inline void SetPosition(const glm::vec3& position);

		virtual inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		virtual inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		virtual void OnEvent(Event& e) = 0;

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
		OrthographicCamera(float aspectRatio, float zoomLevel = 1.0f, float zNear = -1.0f, float zFar = 10.0f);

		virtual inline float GetRenderDistance() const override { return m_ZFar; }

		const inline glm::vec3 GetForward() const { return glm::vec3(0.0f, 0.0f, -1.0f); }
		const inline float GetZoomLevel() const { return m_ZoomLevel; }
		const inline float GetRotation() const { return m_ShouldRotate; }

		void SetRotation(float rotation);
		void SetZoomLevel(float newZoom);

		virtual void OnEvent(Event& e) override;

	private:
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateProjectionMatrix() override;

		bool OnWindowResized(WindowResizeEvent& e);

	private:
		// Since this is an orthographic camera we only need rotation around one axis.
		float m_ShouldRotate = 0.0f;

		float m_MinZoomLevel, m_MaxZoomLevel, m_ZoomLevel;
		float m_AspectRatio;
		float m_ZNear, m_ZFar;

	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float aspectRatio, float fov = 45.0f, float zoomLevel = 1.0f, float zNear = 0.1f, float zFar = 100.0f);
		
		virtual inline float GetRenderDistance() const override { return m_ZFar; }

		inline const glm::vec3 GetRight() const { return m_Right; }
		inline const glm::vec3 GetUp() const { return m_Up; }
		inline const glm::vec3 GetForward() const { return m_Forward; }
		inline float GetFOV() const { return m_FOV; }

		/* TODO: Use quaternions */
		void SetRotationOffset(float x, float y, float z);
		void SetRotationOffset(const glm::vec3& rotationOffset);
		void SetFOV(float newFOV);
		void SetRenderDistance(float newZFar);
		void SetAspectRatio(float newAspectRatio);

		virtual void OnEvent(Event& e) override;

	private:
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateProjectionMatrix() override;

		bool OnWindowResized(WindowResizeEvent& e);

	private:

		float m_MinFOV, m_MaxFOV, m_FOV;
		float m_AspectRatio;
		float m_ZNear, m_ZFar;
		float m_ZoomSpeedFactor = 0.01f;

		glm::vec3 m_Forward;
		glm::vec3 m_Right;
		glm::vec3 m_Up;

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

	};
}
