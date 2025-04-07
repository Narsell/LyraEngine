#pragma once

#include <unordered_map>
#include <vector>

namespace Lyra
{
	class Material;
	class Shader;
	class Texture2D;

	class MaterialLibrary
	{
	public:
		MaterialLibrary() = delete;
		~MaterialLibrary() = default;

		static Ref<Material>& Create(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures = {});
		static Ref<Material>& Create(const std::vector<Ref<Texture2D>> textures);

	private:
		static std::unordered_map<size_t, Ref<Material>> s_LoadedMaterials;
	};
}
