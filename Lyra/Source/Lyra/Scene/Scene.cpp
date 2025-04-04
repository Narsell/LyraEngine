#include "lrpch.h"

#include "Scene.h"

namespace Lyra
{
	void Scene::UploadUniforms(const Ref<Shader>& shader) const
	{
		shader->UploadUniform_Mat4f("u_VP", m_Data.camera->GetViewProjectionMatrix());
		shader->UploadUniform_Mat4f("u_View", m_Data.camera->GetViewMatrix());

		/* Directional light uniforms */
		shader->UploadUniform_3f("u_DirLight.direction", glm::mat3(m_Data.camera->GetViewMatrix()) * m_Data.dirLight.direction);
		shader->UploadUniform_3f("u_DirLight.ambient", m_Data.dirLight.ambient);
		shader->UploadUniform_3f("u_DirLight.diffuse", m_Data.dirLight.diffuse);
		shader->UploadUniform_3f("u_DirLight.specular", m_Data.dirLight.specular);

		/* Point light uniforms*/
		for (int i = 0; i < m_Data.pointLights.size(); i++)
		{
			//TODO: Get rid of this awfully slow std::format omg
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].position", i), glm::vec3(m_Data.camera->GetViewMatrix() * glm::vec4(m_Data.pointLights[i].position, 1.0f)));
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].ambient", i), m_Data.pointLights[i].ambient);
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].diffuse", i), m_Data.pointLights[i].diffuse);
			shader->UploadUniform_3f(std::format("u_PointLights[{0}].specular", i), m_Data.pointLights[i].specular);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].constAttenuation", i), m_Data.pointLights[i].constAttenuation);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].linearAttenuation", i), m_Data.pointLights[i].linearAttenuation);
			shader->UploadUniform_1f(std::format("u_PointLights[{0}].quadAttenuation", i), m_Data.pointLights[i].quadAttenuation);
		}

		/* Spot light uniforms */
		shader->UploadUniform_3f("u_SpotLight.position", m_Data.spotLight.position);
		shader->UploadUniform_3f("u_SpotLight.direction", m_Data.spotLight.direction);
		shader->UploadUniform_1f("u_SpotLight.innerCutoffCosine", glm::cos(glm::radians(m_Data.spotLight.innerCutoffAngle)));
		shader->UploadUniform_1f("u_SpotLight.outerCutoffCosine", glm::cos(glm::radians(m_Data.spotLight.outerCutoffAngle)));
		shader->UploadUniform_3f("u_SpotLight.ambient", m_Data.spotLight.ambient);
		shader->UploadUniform_3f("u_SpotLight.diffuse", m_Data.spotLight.diffuse);
		shader->UploadUniform_3f("u_SpotLight.specular", m_Data.spotLight.specular);
		shader->UploadUniform_1f("u_SpotLight.constAttenuation", m_Data.spotLight.constAttenuation);
		shader->UploadUniform_1f("u_SpotLight.linearAttenuation", m_Data.spotLight.linearAttenuation);
		shader->UploadUniform_1f("u_SpotLight.quadAttenuation", m_Data.spotLight.quadAttenuation);

		m_LastShaderHashUploaded = shader->GetHash();
	}
}