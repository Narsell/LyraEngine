#include "lrpch.h"

#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Lyra
{
	std::unordered_map<std::string, Ref<Texture2D>> Texture2D::s_TexturesLoaded;

	Ref<Texture2D> Lyra::Texture2D::Create(const std::string& path, TextureType textureType)
	{
		if (s_TexturesLoaded.find(path) != s_TexturesLoaded.end())
		{
			return s_TexturesLoaded[path];
		}

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				Ref<Texture2D> texture = std::make_shared<OpenGLTexture2D>(path, textureType);
				s_TexturesLoaded[path] = texture;
				return texture;
			}
			case RendererAPI::API::None:
			{
				LR_CORE_ASSERT(false, "No Renderer API was selected! (RendererAPI::API::None)");
				return nullptr;
			}
			default:
			{
				LR_CORE_ASSERT(false, "Invalid Renderer API");
				return nullptr;
			}
		}
	}
}
