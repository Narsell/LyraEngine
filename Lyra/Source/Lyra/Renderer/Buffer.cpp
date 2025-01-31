#include "lrpch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Lyra
{
	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size, const VertexLayout& layout)
	{
		switch (Renderer::GetApi())
		{
			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size, layout);
			}
			case RendererAPI::None:
			{
				LR_CORE_ASSERT(false, "No Renderer API was selected! (RendererAPI::None)");
				return nullptr;
			}
			default:
			{
				LR_CORE_ASSERT(false, "Invalid Renderer API");
				return nullptr;
			}
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetApi())
		{
			case RendererAPI::OpenGL:
			{
				return new OpenGLIndexBuffer(indices, count);
			}
			case RendererAPI::None:
			{
				LR_CORE_ASSERT(false, "No Renderer API was selected! (RendererAPI::None)");
				return nullptr;
			}
			default:
			{
				LR_CORE_ASSERT(false, "Invalid Renderer API");
				return nullptr;
			}
		}
	}

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetApi())
		{
			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexArray();
			}
			case RendererAPI::None:
			{
				LR_CORE_ASSERT(false, "No Renderer API was selected! (RendererAPI::None)");
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