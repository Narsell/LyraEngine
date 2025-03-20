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

		for (const Ref<Texture2D>& texture : m_Textures)
		{
			Utils::Hash::HashCombine(m_Hash, texture->GetHash());
		}
		Utils::Hash::HashCombine(m_Hash, m_Shader->GetHash(), m_Shininess);
	}

	void Material::SetTexture(const Ref<Texture2D>& texture)
	{
		m_Textures.push_back(texture);
	}

	void Material::UpdateData()
	{
		for (const Ref<Texture2D>& texture : m_Textures)
		{
			texture->Bind();
		}
	}
}