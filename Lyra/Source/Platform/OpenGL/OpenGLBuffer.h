#pragma once

#include "Lyra/Renderer/Buffer.h"

#include <memory>

namespace Lyra
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, size_t size, const VertexLayout& layout);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const VertexLayout& GetLayout() const override { return m_VertexLayout; };

	private:
		const VertexLayout& m_VertexLayout;

	private:
		uint32_t m_RendererId;
	};


	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* vertices, uint32_t count);

		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Count;
	};


	class OpenGLVertexArray final : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		/* Sets the Vertex Atrib Pointer in OpenGL for every Layout's VertexElement */
		virtual void SetupLayout(const VertexLayout& layout) const;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	private:
		uint32_t m_RendererId;
	};
}
