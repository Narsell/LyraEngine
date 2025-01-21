#include "lrpch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Lyra
{
	/* OPEN GL VERTEX BUFFER */

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size)
	{
		//Create vertex buffer and bind it to GL_ARRAY_BUFFER
		glCreateBuffers(1, &m_RendererId);
		Bind();
		// Setting bufferData to GL_ARRAY_BUFFER that we just bound.
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/* OPEN GL INDEX BUFFER */

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		//Create, bind and set indices to GL_ELEMENT_ARRAY_BUFFER which is the index buffer
		glCreateBuffers(1, &m_RendererId);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
