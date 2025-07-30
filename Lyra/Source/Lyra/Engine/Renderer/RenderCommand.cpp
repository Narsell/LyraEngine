#include "lrpch.h"

#include "glad/glad.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/Renderer/OpenGLRendererAPI.h"
#include "Renderer/Renderer.h"

namespace Lyra
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

	RenderCommand::RenderCommand(const RenderCommandData& commandData)
		: m_CommandData(commandData) {}

	RenderCommand::RenderCommand(VertexArray* vertexArray, const Ref<Material>& material, const Ref<const Scene>& scene, const glm::mat4& transform, bool drawIndexed, RenderType renderType)
	{
		m_CommandData.vertexArray = vertexArray;
		m_CommandData.material = material;
		m_CommandData.scene = scene;
		m_CommandData.transform = transform;
		m_CommandData.drawIndexed = drawIndexed;
		m_CommandData.renderType = renderType;
	}

	void RenderCommand::Execute()
	{
		UploadSceneUniforms();
		UploadMaterialUniforms();
		UploadEntityUniforms();

		if (m_CommandData.vertexArray)
		{
			s_RendererAPI->DrawVertices(m_CommandData.vertexArray, m_CommandData.drawIndexed);
		}
	}

	void RenderCommand::UploadSceneUniforms()
	{
		Ref<Shader> shader = m_CommandData.material->GetShader();
		// Only upload scene shader uniforms once per frame or if another shader uploaded these uniforms then I *think* we need to upload again.
		// Not 100% sure though, TODO: Not a big overhead but check this later when I'm wiser.
		
		if (shader->GetHash() != m_LastSceneShaderBound)			
		{	
			const Ref<const Camera>& sceneCamera = m_CommandData.scene->GetCamera();
		
			shader->Bind();
			shader->UploadUniform_Mat4f("u_Proj", sceneCamera->GetProjectionMatrix());
			
			if (m_CommandData.renderType == RenderType::LR_SKYBOX)
			{
				shader->UploadUniform_Mat4f("u_View", glm::mat3(sceneCamera->GetViewMatrix()));
			}
			else
			{
				shader->UploadUniform_Mat4f("u_View", sceneCamera->GetViewMatrix());
			}

			if (shader->HasLightData())
			{
				const DirectionalLight& sceneDirLight = m_CommandData.scene->GetDirectionalLight();
				auto& scenePointLights = m_CommandData.scene->GetPointLights();
				const SpotLight& sceneSpotLight = m_CommandData.scene->GetSpotLight();
				const glm::mat4& viewMatrix = sceneCamera->GetViewMatrix();

				/* Directional light uniforms */
				shader->UploadUniform_3f("u_DirLight.direction", glm::mat3(viewMatrix) * sceneDirLight.direction);
				shader->UploadUniform_3f("u_DirLight.ambient", sceneDirLight.ambient);
				shader->UploadUniform_3f("u_DirLight.diffuse", sceneDirLight.diffuse);
				shader->UploadUniform_3f("u_DirLight.specular", sceneDirLight.specular);

				/* Point light uniforms*/
				for (int i = 0; i < scenePointLights.size(); i++)
				{
					shader->UploadUniform_3f((std::string("u_PointLights[") + std::to_string(i) + "].position"), glm::vec3(viewMatrix * glm::vec4(scenePointLights[i].position, 1.0f)));
					shader->UploadUniform_3f((std::string("u_PointLights[") + std::to_string(i) + "].ambient"), scenePointLights[i].ambient);
					shader->UploadUniform_3f((std::string("u_PointLights[") + std::to_string(i) + "].diffuse"), scenePointLights[i].diffuse);
					shader->UploadUniform_3f((std::string("u_PointLights[") + std::to_string(i) + "].specular"), scenePointLights[i].specular);
					shader->UploadUniform_1f((std::string("u_PointLights[") + std::to_string(i) + "].constAttenuation"), scenePointLights[i].constAttenuation);
					shader->UploadUniform_1f((std::string("u_PointLights[") + std::to_string(i) + "].linearAttenuation"), scenePointLights[i].linearAttenuation);
					shader->UploadUniform_1f((std::string("u_PointLights[") + std::to_string(i) + "].quadAttenuation"), scenePointLights[i].quadAttenuation);
				}

				/* Spot light uniforms */
				shader->UploadUniform_3f("u_SpotLight.position", sceneSpotLight.position);
				shader->UploadUniform_3f("u_SpotLight.direction", sceneSpotLight.direction);
				shader->UploadUniform_1f("u_SpotLight.innerCutoffCosine", glm::cos(glm::radians(sceneSpotLight.innerCutoffAngle)));
				shader->UploadUniform_1f("u_SpotLight.outerCutoffCosine", glm::cos(glm::radians(sceneSpotLight.outerCutoffAngle)));
				shader->UploadUniform_3f("u_SpotLight.ambient", sceneSpotLight.ambient);
				shader->UploadUniform_3f("u_SpotLight.diffuse", sceneSpotLight.diffuse);
				shader->UploadUniform_3f("u_SpotLight.specular", sceneSpotLight.specular);
				shader->UploadUniform_1f("u_SpotLight.constAttenuation", sceneSpotLight.constAttenuation);
				shader->UploadUniform_1f("u_SpotLight.linearAttenuation", sceneSpotLight.linearAttenuation);
				shader->UploadUniform_1f("u_SpotLight.quadAttenuation", sceneSpotLight.quadAttenuation);
			}
			m_LastSceneShaderBound = shader->GetHash();
		}
	}

	void RenderCommand::UploadMaterialUniforms()
	{
		// Only bind new texture slots if the material has changed.
		if (!m_CommandData.material->IsCurrentlyBound())
		{
			m_CommandData.material->UploadUniforms();
			m_CommandData.material->BindTextures();
		}
	}

	void RenderCommand::UploadEntityUniforms()
	{
		Ref<Shader> shader = m_CommandData.material->GetShader();

		if (!shader->IsCurrentlyBound())
		{
			shader->Bind();
		}

		if (m_CommandData.renderType == RenderType::LR_SKYBOX)
		{
			const float cameraRenderDistance = m_CommandData.scene->GetCamera()->GetRenderDistance();
			const float skyboxScaleBounds = cameraRenderDistance / 2.0f;
			shader->UploadUniform_Mat4f("u_Model", glm::scale(glm::mat4(1.0), glm::vec3(skyboxScaleBounds)));
		}
		else
		{
			shader->UploadUniform_Mat4f("u_Model", m_CommandData.transform);
		}

		if (shader->HasLightData())
		{
		// Note for a confused future me looking at this wizardry:
		// 		The normal matrix is the upper 3x3 matrix of the transpose of the inverse of the model matrix. (Multiplied by the view matrix in this case because shader is in view-coords)
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_CommandData.scene->GetCamera()->GetViewMatrix() * m_CommandData.transform)));
			shader->UploadUniform_Mat3f("u_Normal", normalMatrix);
		}
	}
}