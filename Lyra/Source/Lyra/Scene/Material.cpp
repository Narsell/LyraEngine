#include "lrpch.h"

#include "Core/Utils.h"
#include "Scene/Material.h"
#include "Scene/Texture.h"
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

	Material::Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>>& textures, const MaterialProps& matProps)
		 : m_Props(matProps),
		   m_Shader(shader),
		   m_Textures(textures)
	{
		SetCalculatedHash();
	}

	Material::Material(const std::vector<Ref<Texture2D>> textures)
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
		for (const Ref<Texture2D>& texture : m_Textures)
		{
			texture->Bind();
		}
		s_LastBoundMatHash = m_Hash;
	}

	void Material::SetCalculatedHash()
	{
		size_t textureListHash = 0;
		for (const Ref<Texture2D>& texture : m_Textures)
		{
			Utils::Hash::HashCombine(textureListHash, texture->GetHash());
		}
		m_Hash = Utils::Material::CalculateHash(m_Shader->GetHash(), textureListHash, m_Props.shininess);
	}
}