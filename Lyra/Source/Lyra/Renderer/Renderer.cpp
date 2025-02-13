#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Lyra
{
	glm::mat4 Renderer::s_ViewProjectionMatrix = glm::mat4(1.0f);

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniform_Mat4f("u_VP", s_ViewProjectionMatrix);
		shader->UploadUniform_Mat4f("u_Model", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}