#include "lrpch.h"

#include "Scene/Material.h"
#include "Core/Utils.h"

namespace Lyra
{
	Material::Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures)
		:	m_Shininess(32.0f),
			m_Shader(shader),
			m_Textures(textures)
	{
		m_Shader->Bind();
		m_Shader->UploadUniform_1f("u_Material.shininess", m_Shininess);

		std::string texturesHashKey;
		for (const Ref<Texture2D>& texture : m_Textures)
		{
			texturesHashKey += texture->GetPath();
		}

		Utils::Hash::HashCombine(m_Hash, m_Shader->GetHash(), texturesHashKey, m_Shininess);
	}

	void Material::SetTexture(const Ref<Texture2D>& texture)
	{
		m_Textures.push_back(texture);
	}

	void Material::UpdateData()
	{
		//No need to upload the diffuse and specular samplers every time since we're now using layout bindings on those samplers in the shader.
		//I will leave this here just in case :)
		//for (const auto& texture : m_Textures)
		//{
		//	m_Shader->UploadUniform_1i(std::format("u_Material.{0}", Utils::TextureTypeToString(texture->GetType())), Utils::GetTextureTypeSlot(texture->GetType()));
		//}
	}
}