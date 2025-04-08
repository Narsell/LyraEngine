#include "lrpch.h"

#include "MaterialLibrary.h"
#include "Core/Utils.h"
#include "Renderer/Shader.h"
#include "Assets/ShaderLibrary.h"
#include "Scene/Texture.h"

namespace Lyra
{
	std::unordered_map<size_t, Ref<Material>> MaterialLibrary::s_LoadedMaterials;

	Ref<Material>& MaterialLibrary::Create(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures, const MaterialProps& matProps)
	{
		size_t textureListHash = 0;
		for (const Ref<Texture2D>& texture : textures)
		{
			Utils::Hash::HashCombine(textureListHash, texture->GetHash());
		}
		size_t materialHash = Utils::Material::CalculateHash(shader->GetHash(), textureListHash, matProps.shininess);
		if (s_LoadedMaterials.find(materialHash) == s_LoadedMaterials.end())
		{
			s_LoadedMaterials.emplace(materialHash, std::make_shared<Material>(shader, textures, matProps));
		}
		return s_LoadedMaterials.at(materialHash);
	}

	Ref<Material>& MaterialLibrary::Create(const std::vector<Ref<Texture2D>> textures, const MaterialProps& matProps)
	{
		return MaterialLibrary::Create(ShaderLibrary::GetDefaultShader(), textures, matProps);
	}

	Ref<Material>& MaterialLibrary::Create(const MaterialProps& matProps)
	{
		return MaterialLibrary::Create({}, matProps);
	}

}