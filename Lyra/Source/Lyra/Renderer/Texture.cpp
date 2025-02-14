#include "lrpch.h"

#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Lyra
{
	Ref<Texture2D> Lyra::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(path);
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
