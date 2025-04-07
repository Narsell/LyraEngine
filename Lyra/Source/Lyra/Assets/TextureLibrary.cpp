#include "lrpch.h"

#include "TextureLibrary.h"
#include "Scene/Texture.h"
#include "Core/Utils.h"

namespace Lyra
{
	std::unordered_map<std::size_t, Ref<Texture2D>> TextureLibrary::s_TexturesLoaded;

	Ref<Texture2D>& TextureLibrary::Load(const std::string& texturePath, const Texture2DProps& textureProps)
	{
		size_t textureHash = Utils::Texture::CalculateHash(texturePath);
		if (s_TexturesLoaded.find(textureHash) == s_TexturesLoaded.end())
		{
			s_TexturesLoaded.emplace(textureHash, Texture2D::Create(texturePath, textureProps));
		}
		return s_TexturesLoaded.at(textureHash);
	}
}