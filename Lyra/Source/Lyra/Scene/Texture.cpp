#include "lrpch.h"

#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Lyra
{
	std::unordered_map<std::string, Ref<Texture2D>> Texture2D::s_TexturesLoaded;

	Texture2D::Texture2D(const std::string& texturePath, const Texture2DProps& textureProps)
		: m_Path(texturePath), m_Props(textureProps)
	{
		if (!Utils::Texture::IsValidTextureType(m_Props.Type))
		{
			LR_CORE_WARN("Texture at path '{0}' was not given a valid type. Won't bind this texture.");
		}
	}

	Ref<Texture2D> Lyra::Texture2D::Create(const std::string& texturePath, const Texture2DProps& textureProps)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				Ref<Texture2D> texture = std::make_shared<OpenGLTexture2D>(texturePath, textureProps);
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
