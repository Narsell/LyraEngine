#include "lrpch.h"

#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Lyra/Renderer/Renderer.h"

namespace Lyra
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLShader(vertexSrc, fragmentSrc);
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