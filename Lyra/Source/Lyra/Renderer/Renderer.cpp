#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace Lyra
{
	glm::mat4 Renderer::s_VPmatrix = glm::mat4(1.0f);

	void Renderer::BeginScene(const Camera& camera)
	{
		glm::mat4 view = camera.GetTransform();
		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		s_VPmatrix = proj * view;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}