#pragma once

#include "Core/Ref.h"
#include "Renderer/Buffer.h"

namespace Lyra
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, size_t size, const VertexLayout& layout);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetVertexCount() const override { return m_VertexCount; }
		virtual const VertexLayout& GetLayout() const override { return m_VertexLayout; };

	private:
		const VertexLayout& m_VertexLayout;
		uint32_t m_VertexCount;

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

		virtual const uint32_t GetRendererId() const { return m_RendererId; }
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

		virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
		virtual void AddIndexBuffer(IndexBuffer* indexBuffer) override;

		virtual const uint32_t GetRendererId() const { return m_RendererId; }
		virtual const std::vector<Scope<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Scope<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		/* Sets the Vertex Atrib Pointer in OpenGL for every Layout's VertexElement */
		virtual void SetupLayout(const VertexLayout& layout) const;

	private:
		std::vector<Scope<VertexBuffer>> m_VertexBuffers;
		Scope<IndexBuffer> m_IndexBuffer;

	private:
		uint32_t m_RendererId;
	};
}
