#include "lrpch.h"

#include "Material.h"

namespace Lyra
{
	Material::Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures)
		:	m_Shininess(32.0f),
			m_Shader(shader),
			m_Textures(textures),
			m_Hash(0)
	{
		// A material supports only as many textures as unique TextureType elements. (one of each)
		m_Textures.reserve(Utils::GetTotalTextureTypeCount());

		m_Shader->Bind();
		m_Shader->UploadUniform_1f("u_Material.shininess", m_Shininess);

		//TODO: Validate max texture slots, maybe not here though hHMMM
		//GLint maxSlots;
		//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlots);
	}

	void Material::SetTexture(const Ref<Texture2D>& texture)
	{
		m_Textures.push_back(texture);
	}

	void Material::UploadData()
	{
		//No need to upload the diffuse and specular samplers every time since we're now using layout bindings on those samplers in the shader.
		//I will leave this here just in case :)
		//for (const auto& texture : m_Textures)
		//{
		//	m_Shader->UploadUniform_1i(std::format("u_Material.{0}", Utils::TextureTypeToString(texture->GetType())), Utils::GetTextureTypeSlot(texture->GetType()));
		//}
	}
}