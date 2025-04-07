#include "lrpch.h"

#include "MaterialLibrary.h"
#include "Core/Utils.h"
#include "Renderer/Shader.h"
#include "Assets/ShaderLibrary.h"
#include "Scene/Material.h"
#include "Scene/Texture.h"

namespace Lyra
{
	std::unordered_map<size_t, Ref<Material>> MaterialLibrary::s_LoadedMaterials;

	Ref<Material>& MaterialLibrary::Create(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures)
	{
		size_t textureListHash = 0;
		for (const Ref<Texture2D>& texture : textures)
		{
			Utils::Hash::HashCombine(textureListHash, texture->GetHash());
		}
		// TODO: Shininess needs to be part of a struct of material properties!
		size_t materialHash = Utils::Material::CalculateHash(shader->GetHash(), textureListHash, 32.0f);
		if (s_LoadedMaterials.find(materialHash) == s_LoadedMaterials.end())
		{
			s_LoadedMaterials.emplace(materialHash, std::make_shared<Material>(shader, textures));
		}
		return s_LoadedMaterials.at(materialHash);
	}

	Ref<Material>& MaterialLibrary::Create(const std::vector<Ref<Texture2D>> textures)
	{
		return MaterialLibrary::Create(ShaderLibrary::GetDefaultShader(), textures);
	}

}