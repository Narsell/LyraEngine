#pragma once

#include <unordered_map>
#include <vector>

#include "Scene/Material.h"

namespace Lyra
{
	class Shader;
	class Texture2D;

	class MaterialLibrary
	{
	public:
		MaterialLibrary() = delete;
		~MaterialLibrary() = default;

		static Ref<Material>& Create(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures = {}, const MaterialProps& matProps = {});
		static Ref<Material>& Create(const std::vector<Ref<Texture2D>> textures, const MaterialProps& matProps = {});
		static Ref<Material>& Create(const MaterialProps& matProps = {});

	private:
		static std::unordered_map<size_t, Ref<Material>> s_LoadedMaterials;
	};
}
