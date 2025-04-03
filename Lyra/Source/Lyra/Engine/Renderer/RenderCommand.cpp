#include "lrpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Renderer/Renderer.h"

namespace Lyra
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
	size_t RenderCommand::s_LastBoundShaderHash = 0;
	size_t RenderCommand::s_LastBoundMaterialHash = 0;

	RenderCommand::RenderCommand(const RenderCommandData& commandData)
		: m_CommandData(commandData) {}

	RenderCommand::RenderCommand(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const Ref<const SceneProps>& sceneProps, const glm::mat4& transform, bool drawIndexed, RenderType renderType)
	{
		m_CommandData.vertexArray = vertexArray;
		m_CommandData.material = material;
		m_CommandData.sceneProps = sceneProps;
		m_CommandData.transform = transform;
		m_CommandData.drawIndexed = drawIndexed;
		m_CommandData.renderType = renderType;
	}

	void RenderCommand::Execute()
	{
		SetSceneUniforms();
		SetMaterialUniforms();
		SetEntityUniforms();

		s_RendererAPI->DrawVertices(m_CommandData.vertexArray, m_CommandData.drawIndexed);
	}

	void RenderCommand::SetSceneUniforms()
	{
		Ref<Shader> shader = m_CommandData.material->GetShader();
		const Ref<const SceneProps>& sceneProps = m_CommandData.sceneProps;

		if (shader->GetHash() != s_LastBoundShaderHash)
		{
			shader->Bind();
			s_LastBoundShaderHash = shader->GetHash();
		}

		shader->UploadUniform_Mat4f("u_VP", sceneProps->Camera->GetViewProjectionMatrix());
		shader->UploadUniform_Mat4f("u_View", sceneProps->Camera->GetViewMatrix());

		/* Directional light uniforms */
		shader->UploadUniform_3f("u_DirLight.direction", glm::mat3(sceneProps->Camera->GetViewMatrix()) * sceneProps->DirLight.direction);
		shader->UploadUniform_3f("u_DirLight.ambient", sceneProps->DirLight.ambient);
		shader->UploadUniform_3f("u_DirLight.diffuse", sceneProps->DirLight.diffuse);
		shader->UploadUniform_3f("u_DirLight.specular", sceneProps->DirLight.specular);

		/* Point light uniforms*/
		for (int i = 0; i < sceneProps->PointLights.size(); i++)
		{
			//TODO: Get rid of this awfully slow std::format omg
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].position", i), glm::vec3(sceneProps->Camera->GetViewMatrix() * glm::vec4(sceneProps->PointLights[i].position, 1.0f)));
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].ambient", i), sceneProps->PointLights[i].ambient);
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].diffuse", i), sceneProps->PointLights[i].diffuse);
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].specular", i), sceneProps->PointLights[i].specular);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].constAttenuation", i), sceneProps->PointLights[i].constAttenuation);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].linearAttenuation", i), sceneProps->PointLights[i].linearAttenuation);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].quadAttenuation", i), sceneProps->PointLights[i].quadAttenuation);
		}

		/* Spot light uniforms */
		shader->UploadUniform_3f("u_SpotLight.position", sceneProps->SpotLight.position);
		shader->UploadUniform_3f("u_SpotLight.direction", sceneProps->SpotLight.direction);
		shader->UploadUniform_1f("u_SpotLight.innerCutoffCosine", glm::cos(glm::radians(sceneProps->SpotLight.innerCutoffAngle)));
		shader->UploadUniform_1f("u_SpotLight.outerCutoffCosine", glm::cos(glm::radians(sceneProps->SpotLight.outerCutoffAngle)));
		shader->UploadUniform_3f("u_SpotLight.ambient", sceneProps->SpotLight.ambient);
		shader->UploadUniform_3f("u_SpotLight.diffuse", sceneProps->SpotLight.diffuse);
		shader->UploadUniform_3f("u_SpotLight.specular", sceneProps->SpotLight.specular);
		shader->UploadUniform_1f("u_SpotLight.constAttenuation", sceneProps->SpotLight.constAttenuation);
		shader->UploadUniform_1f("u_SpotLight.linearAttenuation", sceneProps->SpotLight.linearAttenuation);
		shader->UploadUniform_1f("u_SpotLight.quadAttenuation", sceneProps->SpotLight.quadAttenuation);
	}

	void RenderCommand::SetMaterialUniforms()
	{
		// Only bind new texture slots if the material has changed.
		if (m_CommandData.material->GetHash() != s_LastBoundMaterialHash)
		{
			m_CommandData.material->BindTextures();
			s_LastBoundMaterialHash = m_CommandData.material->GetHash();
		}
	}

	void RenderCommand::SetEntityUniforms()
	{
		Ref<Shader> shader = m_CommandData.material->GetShader();

		shader->UploadUniform_Mat4f("u_Model", m_CommandData.transform);
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_CommandData.sceneProps->Camera->GetViewMatrix() * m_CommandData.transform)));
		shader->UploadUniform_Mat3f("u_Normal", normalMatrix);
	}
}