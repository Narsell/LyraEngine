#include "lrpch.h"

#include "Scene/Material.h"
#include "Core/Utils.h"

namespace Lyra
{
	size_t Material::s_LastBoundMaterialHash = 0;

	Material::Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures)
		:	m_Shininess(32.0f),
			m_Shader(shader),
			m_Textures(textures)
	{
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

	void Material::UploadUniforms() const
	{
		if (!m_Shader->IsCurrentlyBound())
		{
			m_Shader->Bind();
		}
		m_Shader->UploadUniform_1f("u_Material.shininess", m_Shininess);
	}

	void Material::BindTextures() const
	{
		for (const Ref<Texture2D>& texture : m_Textures)
		{
			texture->Bind();
		}
		s_LastBoundMaterialHash = m_Hash;
	}
}