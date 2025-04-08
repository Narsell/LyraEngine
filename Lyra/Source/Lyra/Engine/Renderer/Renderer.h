#pragma once

#include "Core/Ref.h"
#include "RendererAPI.h"
#include "RenderCommandQueue.h"

namespace Lyra
{
	class Material;
	class Shader;
	class Scene;

	class Renderer
	{
	public:
		static void Init();

		static uint32_t GetDrawCallCount() { return s_LastDrawCallCount; }
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Ref<const Scene>& scene);
		static void EndScene();
		static void Submit(const Ref<Material>& material, const Scope<VertexArray>& vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f), bool drawIndexed = true);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static Ref<const Scene> s_Scene;

		static RenderCommandQueue s_RenderQueue;

		static uint32_t s_LastDrawCallCount;
		static uint32_t s_CurrentDrawCallCount;
	};
}