#pragma once

#include "Lyra.h"
#include "imgui.h"

namespace Lyra
{
	enum class EDITOR_INPUT_MODE : uint8_t
	{
		VIEWPORT_MODE,
		EDITING_MODE
	};

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(const KeyPressedEvent& e);
		void SetEditorInputMode(EDITOR_INPUT_MODE inputMode);

	private:
		PerspectiveCameraController m_CameraController;

		Ref<Model> m_SponzaModel;
		Ref<Model> m_BackpackModel;

		Ref<Entity> m_SponzaObj;
		Ref<Entity> m_BackpackObj;

		Scope<VertexArray> m_LightSourceCubeVertexArray;
		Ref<Shader> m_LightSourceShader;

		Ref<Scene> m_Scene;
		float m_LightSourceAngle;
		float m_LightSourceSpeed;

		float m_LastFrameTime = 0.0f;
		float m_Time = 0.0f;

		glm::vec2 m_ViewportSize = glm::vec2();

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Window* m_Window;
	};
}