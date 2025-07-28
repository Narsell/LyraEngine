#include "lrpch.h"

#include "Core/Utils.h"
#include "Assets/Material.h"
#include "Assets/Texture.h"
#include "Renderer/Shader.h"
#include "Assets/ShaderLibrary.h"

namespace Lyra
{
	size_t Material::s_LastBoundMatHash = 0;

	Material::Material()
		: m_Shader(ShaderLibrary::GetDefaultShader())
	{
		SetCalculatedHash();
	}

	Material::Material(const Ref<Shader>& shader, const std::vector<Ref<Texture>>& textures, const MaterialProps& matProps)
		 : m_Props(matProps),
		   m_Shader(shader),
		   m_Textures(textures)
	{
		SetCalculatedHash();
	}

	Material::Material(const std::vector<Ref<Texture>> textures)
		: Material(ShaderLibrary::GetDefaultShader(), textures, {})
	{
	}

	void Material::UploadUniforms() const
	{
		if (!m_Shader->IsCurrentlyBound())
		{
			m_Shader->Bind();
		}
		m_Shader->UploadUniform_1f("u_Material.shininess", m_Props.shininess);
	}

	void Material::BindTextures() const
	{
		for (const Ref<Texture>& texture : m_Textures)
		{
			texture->Bind();
		}
		s_LastBoundMatHash = m_Hash;
	}

	void Material::SetCalculatedHash()
	{
		// TODO: Make Utils::Hash more useful by inclusing texture, material, etc.. hash creation functions.
		size_t textureListHash = 0;
		for (const Ref<Texture>& texture : m_Textures)
		{
			Utils::Hash::HashCombine(textureListHash, texture->GetHash());
		}
		m_Hash = Utils::Material::CalculateHash(m_Shader->GetHash(), textureListHash, m_Props.shininess);
	}
}