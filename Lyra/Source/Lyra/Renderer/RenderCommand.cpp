#include "lrpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Lyra
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}