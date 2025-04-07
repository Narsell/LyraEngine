#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Assets/ShaderLibrary.h"

namespace Lyra
{
	RenderCommandQueue Renderer::s_RenderQueue;
	Ref<const Scene> Renderer::s_Scene;
	uint32_t Renderer::s_CurrentDrawCallCount = 0;
	uint32_t Renderer::s_LastDrawCallCount = 0;

	void Renderer::Init()
	{
		RenderCommand::InitRenderer();
		/* We want to guarantee the default shader exists. */
		ShaderLibrary::Load("Default", "Assets/Shaders/PhongModel.glsl");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Ref<const Scene>& scene)
	{
		s_Scene = scene;
		s_CurrentDrawCallCount = 0;
	}

	void Renderer::EndScene()
	{
		s_RenderQueue.Flush();
		s_Scene->ClearUniformCache();
		s_LastDrawCallCount = s_CurrentDrawCallCount;
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix, bool drawIndexed)
	{
		s_RenderQueue.Enqueue(vertexArray, material, s_Scene, modelMatrix, drawIndexed, RenderType::LR_OPAQUE);
		s_CurrentDrawCallCount++;
	}
}