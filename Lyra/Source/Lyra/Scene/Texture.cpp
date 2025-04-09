#include "lrpch.h"

#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Lyra
{
	Texture2D::Texture2D(const std::filesystem::path& texturePath, const Texture2DProps& textureProps)
		: m_Path(texturePath), m_Props(textureProps)
	{
		if (!Utils::Texture::IsValidTextureType(m_Props.Type))
		{
			LR_CORE_WARN("Texture at path '{0}' was not given a valid type. Won't bind this texture.");
		}
	}

	Ref<Texture2D> Lyra::Texture2D::Create(const std::filesystem::path& texturePath, const Texture2DProps& textureProps)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLTexture2D>(texturePath, textureProps);
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
