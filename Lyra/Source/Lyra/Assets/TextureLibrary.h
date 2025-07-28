#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>

#include "Assets/Texture.h"

namespace Lyra
{
	class TextureLibrary
	{
	public:
		TextureLibrary() = delete;
		~TextureLibrary() = default;

		static Ref<Texture2D> Load2DTexture(const std::filesystem::path& texturePath, const TextureProps& textureProps);
		static Ref<CubemapTexture> LoadCubemapTexture(const std::vector<std::filesystem::path>& texturePaths, const TextureProps& textureProps);

	private:
		static std::unordered_map<std::size_t, Ref<Texture>> s_TexturesLoaded;
	};
}
