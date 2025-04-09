#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>

#include "Scene/Texture.h"

namespace Lyra
{
	class TextureLibrary
	{
	public:
		TextureLibrary() = delete;
		~TextureLibrary() = default;

		static Ref<Texture2D>& Load(const std::filesystem::path& texturePath, const Texture2DProps& textureProps);

	private:
		static std::unordered_map<std::size_t, Ref<Texture2D>> s_TexturesLoaded;
	};
}
