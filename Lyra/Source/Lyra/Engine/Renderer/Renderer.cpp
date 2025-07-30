#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Core/Application.h"
#include "Assets/ShaderLibrary.h"

namespace Lyra
{
	Ref<FrameBuffer> Renderer::s_FrameBuffer; // TODO: Maybe make a dummy framebuffer derived with just inline functions that do nothing
	RenderCommandQueue Renderer::s_RenderQueue;
	Ref<const Scene> Renderer::s_Scene;
	uint32_t Renderer::s_CurrentDrawCallCount = 0;
	uint32_t Renderer::s_LastDrawCallCount = 0;

	void Renderer::Init(const RendererSpecification& spec)
	{
		RenderCommand::InitRenderer(spec);

		if (spec.useFrameBuffer)
		{
			/* Create frame buffer object */
			const Window& window = Application::Get().GetWindow();
			s_FrameBuffer = FrameBuffer::Create(FrameBufferSpecification(window.GetWidth(), window.GetHeight()));
		}
		/* Guarantee the default shader exists. */
		ShaderLibrary::Load("Default", "Assets/Shaders/PhongModel.glsl");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginFrame(const Ref<const Scene>& scene)
	{
		s_Scene = scene;

		if (s_FrameBuffer.get())
		{
			s_FrameBuffer->Bind();
		}
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::Clear();

		s_CurrentDrawCallCount = 0;
	}

	void Renderer::EndFrame()
	{
		s_RenderQueue.Flush();

		if (s_FrameBuffer.get())
		{
			s_FrameBuffer->Unbind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			RenderCommand::Clear();
		}

		s_LastDrawCallCount = s_CurrentDrawCallCount;
	}

	void Renderer::Submit(const Ref<Material>& material, const Scope<VertexArray>& vertexArray, const glm::mat4& modelMatrix, bool drawIndexed, RenderType renderType)
	{
		s_RenderQueue.Enqueue(vertexArray.get(), material, s_Scene, modelMatrix, drawIndexed, renderType);
		s_CurrentDrawCallCount++;
	}
}