#pragma once

#include "Core/Ref.h"
#include "RendererAPI.h"
#include "RenderCommandQueue.h"

namespace Lyra
{
	class Material;
	class Shader;
	class Scene;
	class FrameBuffer;

	class Renderer
	{
	public:
		static void Init(const RendererSpecification& spec);

		static const Ref<FrameBuffer>& GetFrameBuffer() { return s_FrameBuffer; }
		static uint32_t GetDrawCallCount() { return s_LastDrawCallCount; }

		static void BeginFrame(const Ref<const Scene>& scene);
		static void EndFrame();
		static void Submit(const Ref<Material>& material, const Scope<VertexArray>& vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f), bool drawIndexed = true, bool submitNormal = true);
		
		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static Ref<const Scene> s_Scene;
		static Ref<FrameBuffer> s_FrameBuffer;
		static RenderCommandQueue s_RenderQueue;

		static uint32_t s_LastDrawCallCount;
		static uint32_t s_CurrentDrawCallCount;
	};
}