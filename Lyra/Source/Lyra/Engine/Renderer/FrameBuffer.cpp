#include "lrpch.h"

#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/Renderer/OpenGLFrameBuffer.h"

namespace Lyra
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLFrameBuffer>(spec);
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