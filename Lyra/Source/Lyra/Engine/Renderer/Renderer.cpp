#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Lyra
{
	RenderCommandQueue Renderer::s_RenderQueue;
	Ref<const SceneProps> Renderer::s_SceneProps;
	uint32_t Renderer::s_CurrentDrawCallCount = 0;
	uint32_t Renderer::s_LastDrawCallCount = 0;


	void Renderer::Init()
	{
		RenderCommand::InitRenderer();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Ref<const SceneProps>& sceneProps)
	{
		s_SceneProps = sceneProps;
		s_CurrentDrawCallCount = 0;
	}

	void Renderer::EndScene()
	{
		s_RenderQueue.Flush();
		s_LastDrawCallCount = s_CurrentDrawCallCount;
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix, bool drawIndexed)
	{
		//TODO: Get rid of this overload, who in the right mind would heap allocate on every draw call smh...
		//This is only here for compatibility reasons for now.
		Ref<Material> material = std::make_shared<Material>(shader);
		Submit(material, vertexArray, modelMatrix, drawIndexed);
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix, bool drawIndexed)
	{
		// TODO: emplace this instead of copying over
		RenderCommand command = RenderCommand(vertexArray, material, s_SceneProps, modelMatrix, drawIndexed, RenderType::LR_OPAQUE);
		s_RenderQueue.Enqueue(command);
		s_CurrentDrawCallCount++;
	}


}