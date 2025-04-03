#pragma once

#include "Core/Ref.h"
#include "Scene/Camera/Camera.h"
#include "Scene/Scene.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "RenderCommandQueue.h"

namespace Lyra
{

	class Renderer
	{
	public:
		static void Init();

		static uint32_t GetDrawCallCount() { return s_LastDrawCallCount; }
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Ref<const SceneProps>& sceneProps);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f), bool drawIndexed = true);
		static void Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f), bool drawIndexed = true);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static Ref<const SceneProps> s_SceneProps;

		static RenderCommandQueue s_RenderQueue;

		static uint32_t s_LastDrawCallCount;
		static uint32_t s_CurrentDrawCallCount;
	};
}