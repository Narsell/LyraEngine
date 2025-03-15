#include "lrpch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Lyra
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size, const VertexLayout& layout)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(vertices, size, layout);
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

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(indices, count);
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

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexArray>();
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