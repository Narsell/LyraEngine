#include "lrpch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Lyra
{
	/* OPENGL VERTEX BUFFER */

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

	/* OPENGL INDEX BUFFER */

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

	/* OPENGL VERTEX ARRAY */

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
		Bind();
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	static int GetOpenGLType(VertexTypeInfo vertexType)
	{
		switch (vertexType.type)
		{
			case VertexTypeInfo::Type::Float:
				return GL_FLOAT;
			case VertexTypeInfo::Type::UInt:
				return GL_UNSIGNED_INT;
			default:
			{
				LR_CORE_ASSERT(false, "No matching OpenGL type");
				return 0;
			}
		}

	}

	void OpenGLVertexArray::SetLayout(const VertexLayout& layout) const
	{
		auto& vertexElements = layout.GetElements();
		uint32_t offset = 0;

		for (int i = 0; i < vertexElements.size(); i++)
		{
			VertexElement vertexElement = vertexElements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, vertexElement.count, GetOpenGLType(vertexElement.typeInfo), vertexElement.normalized, layout.GetStride(), (const void*)offset);
			offset += vertexElement.GetSize();
		}
	}

}
