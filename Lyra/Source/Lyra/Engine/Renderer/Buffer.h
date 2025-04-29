#pragma once

#include "VertexLayout.h"
#include "Core/Ref.h"

namespace Lyra
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const VertexLayout& GetLayout() const = 0;
		virtual size_t GetVertexCount() const = 0;

		/* Returns pointer to heap allocated VertexBuffer. Assign this to a VertexArray instance to avoid leaks. */
		static VertexBuffer* Create(float* vertices, size_t size, const VertexLayout& layout);
	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const uint32_t GetRendererId() const = 0;
		virtual size_t GetCount() const = 0;

		/* Returns pointer to heap allocated IndexBuffer. Assign this to a VertexArray instance to avoid leaks. */
		static IndexBuffer* Create(uint32_t* indices, size_t count);
	};


	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
		virtual void AddIndexBuffer(IndexBuffer* indexBuffer) = 0;

		virtual const uint32_t GetRendererId() const = 0;
		virtual const std::vector<Scope<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Scope<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}