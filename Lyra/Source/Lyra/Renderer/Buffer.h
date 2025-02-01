#pragma once

#include "VertexLayout.h"

namespace Lyra
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const VertexLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, size_t size, const VertexLayout& layout);

	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};


	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};

}