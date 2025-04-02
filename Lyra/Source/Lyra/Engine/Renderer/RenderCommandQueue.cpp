#include "lrpch.h"

#include "RenderCommandQueue.h"

namespace Lyra
{
	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandQueue.reserve(512);
	}

	void RenderCommandQueue::Enqueue(RenderCommand command)
	{
		// TODO: Sort as I push elements in or sort later? HMM
		m_CommandQueue.push_back(command);
	}

	void RenderCommandQueue::Flush(const SceneProps& sceneProps)
	{
		for (const RenderCommand& command : m_CommandQueue)
		{
			Ref<Material> material = command.GetData().material;
			Ref<Shader> shader = material->GetShader();
			const glm::mat4 transform = command.GetData().transform;

			// TODO: This whole section needs to be moved/refactor, Flush should only execute the commands in the command queue.
			// Ideally the material would handle shader uploading by itself so a single call to Material->Bind() should do everything (bind shader, set uniforms, bind texture slots)
			// Only bind shader and per-frame uniforms if the shader has changed.
			if (shader->GetHash() != m_LastBoundShaderHash)
			{
				shader->Bind();
				shader->UploadUniform_Mat4f("u_VP", sceneProps.Camera->GetViewProjectionMatrix());
				shader->UploadUniform_Mat4f("u_View", sceneProps.Camera->GetViewMatrix());

				/* Directional light uniforms */
				shader->UploadUniform_3f("u_DirLight.direction", glm::mat3(sceneProps.Camera->GetViewMatrix()) * sceneProps.DirLight.direction);
				shader->UploadUniform_3f("u_DirLight.ambient", sceneProps.DirLight.ambient);
				shader->UploadUniform_3f("u_DirLight.diffuse", sceneProps.DirLight.diffuse);
				shader->UploadUniform_3f("u_DirLight.specular", sceneProps.DirLight.specular);

				/* Point light uniforms*/
				for (int i = 0; i < sceneProps.PointLights.size(); i++)
				{
					//TODO: Get rid of this awfully slow std::format omg
					shader->UploadUniform_3f(std::format("u_PointLights[{0}].position", i), glm::vec3(sceneProps.Camera->GetViewMatrix() * glm::vec4(sceneProps.PointLights[i].position, 1.0f)));
					shader->UploadUniform_3f(std::format("u_PointLights[{0}].ambient", i), sceneProps.PointLights[i].ambient);
					shader->UploadUniform_3f(std::format("u_PointLights[{0}].diffuse", i), sceneProps.PointLights[i].diffuse);
					shader->UploadUniform_3f(std::format("u_PointLights[{0}].specular", i), sceneProps.PointLights[i].specular);
					shader->UploadUniform_1f(std::format("u_PointLights[{0}].constAttenuation", i), sceneProps.PointLights[i].constAttenuation);
					shader->UploadUniform_1f(std::format("u_PointLights[{0}].linearAttenuation", i), sceneProps.PointLights[i].linearAttenuation);
					shader->UploadUniform_1f(std::format("u_PointLights[{0}].quadAttenuation", i), sceneProps.PointLights[i].quadAttenuation);
				}

				/* Spot light uniforms */
				shader->UploadUniform_3f("u_SpotLight.position", sceneProps.SpotLight.position);
				shader->UploadUniform_3f("u_SpotLight.direction", sceneProps.SpotLight.direction);
				shader->UploadUniform_1f("u_SpotLight.innerCutoffCosine", glm::cos(glm::radians(sceneProps.SpotLight.innerCutoffAngle)));
				shader->UploadUniform_1f("u_SpotLight.outerCutoffCosine", glm::cos(glm::radians(sceneProps.SpotLight.outerCutoffAngle)));
				shader->UploadUniform_3f("u_SpotLight.ambient", sceneProps.SpotLight.ambient);
				shader->UploadUniform_3f("u_SpotLight.diffuse", sceneProps.SpotLight.diffuse);
				shader->UploadUniform_3f("u_SpotLight.specular", sceneProps.SpotLight.specular);
				shader->UploadUniform_1f("u_SpotLight.constAttenuation", sceneProps.SpotLight.constAttenuation);
				shader->UploadUniform_1f("u_SpotLight.linearAttenuation", sceneProps.SpotLight.linearAttenuation);
				shader->UploadUniform_1f("u_SpotLight.quadAttenuation", sceneProps.SpotLight.quadAttenuation);
			}

			// Only bind new texture slots if the material has changed.
			if (material->GetHash() != m_LastBoundMaterialHash)
			{
				material->BindTextures();
				m_LastBoundMaterialHash = material->GetHash();
			}

			// Upload per-entity uniforms on every draw call.
			shader->UploadUniform_Mat4f("u_Model", transform);
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(sceneProps.Camera->GetViewMatrix() * transform)));
			shader->UploadUniform_Mat3f("u_Normal", normalMatrix);

			command.Execute();
		}

		m_CommandQueue.clear();
	}
}