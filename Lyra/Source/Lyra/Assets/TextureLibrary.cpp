#include "lrpch.h"

#include <memory>

#include "TextureLibrary.h"
#include "Assets/Texture.h"
#include "Core/Utils.h"

namespace Lyra
{
	std::unordered_map<std::size_t, Ref<Texture>> TextureLibrary::s_TexturesLoaded;

	Ref<Texture2D> TextureLibrary::Load2DTexture(const std::filesystem::path& texturePath, const TextureProps& textureProps)
	{
		size_t textureHash = Utils::Texture::CalculateHash(texturePath, textureProps);
		if (s_TexturesLoaded.find(textureHash) == s_TexturesLoaded.end())
		{
			LR_CORE_TRACE("Loading 2D texture at {0}", texturePath.string().c_str());
			s_TexturesLoaded.emplace(textureHash, Texture2D::Create(texturePath, textureProps));
		}
		// Performing a static downcast is safe here because the type is guaranteed!
		return std::static_pointer_cast<Texture2D>(s_TexturesLoaded.at(textureHash));
	}

	Ref<CubemapTexture> TextureLibrary::LoadCubemapTexture(const std::vector<std::filesystem::path>& texturePaths, const TextureProps& textureProps)
	{
		size_t texturesHash = Utils::Texture::CalculateListHash(texturePaths, textureProps);
		if (s_TexturesLoaded.find(texturesHash) == s_TexturesLoaded.end())
		{
			LR_CORE_TRACE("Loading cubemap textures at {0}", texturePaths.front().parent_path().string().c_str());
			s_TexturesLoaded.emplace(texturesHash, CubemapTexture::Create(texturePaths, textureProps));
		}
		// Performing a static downcast is safe here because the type is guaranteed!
		return std::static_pointer_cast<CubemapTexture>(s_TexturesLoaded.at(texturesHash));
	}
}