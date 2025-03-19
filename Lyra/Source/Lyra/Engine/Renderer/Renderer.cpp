#include "lrpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Lyra
{
	SceneProps Renderer::s_SceneProps;
	uint32_t Renderer::s_CurrentDrawCallCount = 0;
	uint32_t Renderer::s_LastDrawCallCount = 0;


	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const SceneProps& sceneProps)
	{
		s_SceneProps = sceneProps;
		s_CurrentDrawCallCount = 0;
	}

	void Renderer::EndScene()
	{
		s_LastDrawCallCount = s_CurrentDrawCallCount;
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix, bool drawIndexed)
	{
		shader->Bind();
		shader->UploadUniform_Mat4f("u_VP", s_SceneProps.Camera->GetViewProjectionMatrix());
		shader->UploadUniform_Mat4f("u_View", s_SceneProps.Camera->GetViewMatrix());
		shader->UploadUniform_Mat4f("u_Model", modelMatrix);

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(s_SceneProps.Camera->GetViewMatrix() * modelMatrix)));
		shader->UploadUniform_Mat3f("u_Normal", normalMatrix);

		/* Directional light uniforms */
		shader->UploadUniform_3f("u_DirLight.direction", glm::mat3(s_SceneProps.Camera->GetViewMatrix()) * s_SceneProps.DirLight.direction);
		shader->UploadUniform_3f("u_DirLight.ambient", s_SceneProps.DirLight.ambient);
		shader->UploadUniform_3f("u_DirLight.diffuse", s_SceneProps.DirLight.diffuse);
		shader->UploadUniform_3f("u_DirLight.specular", s_SceneProps.DirLight.specular);

		/* Point light uniforms*/
		for (int i = 0; i < s_SceneProps.PointLights.size(); i++)
		{
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].position", i), glm::vec3(s_SceneProps.Camera->GetViewMatrix() * glm::vec4(s_SceneProps.PointLights[i].position, 1.0f)));
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].ambient", i), s_SceneProps.PointLights[i].ambient);
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].diffuse", i), s_SceneProps.PointLights[i].diffuse);
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].specular", i), s_SceneProps.PointLights[i].specular);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].constAttenuation", i), s_SceneProps.PointLights[i].constAttenuation);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].linearAttenuation", i), s_SceneProps.PointLights[i].linearAttenuation);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].quadAttenuation", i), s_SceneProps.PointLights[i].quadAttenuation);
		}

		/* Spot light uniforms */
		shader->UploadUniform_3f("u_SpotLight.position", s_SceneProps.SpotLight.position);
		shader->UploadUniform_3f("u_SpotLight.direction", s_SceneProps.SpotLight.direction);
		shader->UploadUniform_1f("u_SpotLight.innerCutoffCosine", glm::cos(glm::radians(s_SceneProps.SpotLight.innerCutoffAngle)));
		shader->UploadUniform_1f("u_SpotLight.outerCutoffCosine", glm::cos(glm::radians(s_SceneProps.SpotLight.outerCutoffAngle)));
		shader->UploadUniform_3f("u_SpotLight.ambient", s_SceneProps.SpotLight.ambient);
		shader->UploadUniform_3f("u_SpotLight.diffuse", s_SceneProps.SpotLight.diffuse);
		shader->UploadUniform_3f("u_SpotLight.specular", s_SceneProps.SpotLight.specular);
		shader->UploadUniform_1f("u_SpotLight.constAttenuation", s_SceneProps.SpotLight.constAttenuation);
		shader->UploadUniform_1f("u_SpotLight.linearAttenuation", s_SceneProps.SpotLight.linearAttenuation);
		shader->UploadUniform_1f("u_SpotLight.quadAttenuation", s_SceneProps.SpotLight.quadAttenuation);

		vertexArray->Bind();

		if (drawIndexed)
		{
			RenderCommand::DrawIndexed(vertexArray);
		}
		else
		{
			RenderCommand::DrawUnindexed(vertexArray);
		}
		s_CurrentDrawCallCount++;
	}
}