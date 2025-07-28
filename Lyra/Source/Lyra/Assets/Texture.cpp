#include "lrpch.h"

#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/Assets/OpenGLTexture.h"
#include "Platform/OpenGL/Assets/OpenGLCubemap.h"

namespace Lyra
{
	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& texturePath, const TextureProps& textureProps)
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

	Ref<CubemapTexture> CubemapTexture::Create(const std::vector<std::filesystem::path>& texturePaths, const TextureProps& textureProps)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLCubemap>(texturePaths, textureProps);
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
