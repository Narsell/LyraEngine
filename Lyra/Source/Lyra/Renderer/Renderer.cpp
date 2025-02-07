#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace Lyra
{
	void Renderer::BeginScene(const Camera& camera, const std::array<std::shared_ptr<Shader>, 2>& shaders)
	{
		glm::mat4 view = camera.GetTransform();
		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		glm::mat4 mvp = proj * view * model;

		/* 
			For every shader we want to upload the same mvp matrix, at least for now. (I KNOW THIS IS NOT GOOD) 
			This should probably be the responsibility of the shader.
		*/
		for (auto& shader : shaders)
		{
			shader->Bind();
			RenderCommand::UploadUniform_Mat4f(shader->GetRendererID(), "u_MVP", mvp);
		}
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