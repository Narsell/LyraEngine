#include "lrpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Lyra
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

	RenderCommand::RenderCommand(const RenderCommandData& commandData)
		: m_CommandData(commandData) {}

	RenderCommand::RenderCommand(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform, bool drawIndexed, RenderType renderType)
	{
		m_CommandData.vertexArray = vertexArray;
		m_CommandData.material = material;
		m_CommandData.transform = transform;
		m_CommandData.drawIndexed = drawIndexed;
		m_CommandData.renderType = renderType;
	}

	void RenderCommand::Execute() const
	{
		s_RendererAPI->DrawVertices(m_CommandData.vertexArray, m_CommandData.drawIndexed);
	}
}