#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Lyra
{
	glm::mat4 Renderer::s_ViewProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 Renderer::s_ViewMatrix = glm::mat4(1.0f);

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		s_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_ViewMatrix = camera.GetViewMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix, bool drawIndexed)
	{
		shader->Bind();
		shader->UploadUniform_Mat4f("u_VP", s_ViewProjectionMatrix);
		shader->UploadUniform_Mat4f("u_View", s_ViewMatrix);
		shader->UploadUniform_Mat4f("u_Model", modelMatrix);

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(s_ViewMatrix * modelMatrix)));
		shader->UploadUniform_Mat3f("u_Normal", normalMatrix);
		

		vertexArray->Bind();

		if (drawIndexed)
		{
			RenderCommand::DrawIndexed(vertexArray);
		}
		else
		{
			RenderCommand::DrawUnindexed(vertexArray);
		}
	}
}