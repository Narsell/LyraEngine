#include "lrpch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Lyra
{

	static int GetOpenGLType(const ShaderData::Type& dataType)
	{
		switch (dataType)
		{
			case ShaderData::Type::Float:	return GL_FLOAT;
			case ShaderData::Type::Float2:	return GL_FLOAT;
			case ShaderData::Type::Float3:	return GL_FLOAT;
			case ShaderData::Type::Float4:	return GL_FLOAT;
			case ShaderData::Type::Int:		return GL_INT;
			case ShaderData::Type::UInt:	return GL_UNSIGNED_INT;
		}
		LR_CORE_ASSERT(false, "Unknown Shader::DataType");
		return 0;
	}


	/* ############################# */
	/* ### OPENGL VERTEX BUFFER #### */
	/* ############################# */

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size, const VertexLayout& layout)
		:	m_VertexLayout(layout),
			m_VertexCount(size/layout.GetStride())
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

	/* ############################# */
	/* #### OPENGL INDEX BUFFER #### */
	/* ############################# */

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		//Create, bind and set indices to GL_ELEMENT_ARRAY_BUFFER which is the index buffer
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
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


	/* ############################# */
	/* #### OPENGL VERTEX ARRAY #### */
	/* ############################# */

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
	{
		/* 
			We first bind the VAO and then the actual vertex buffer 
			That's what links the VAO with the VertexBuffer in OpenGL.
		*/
		Bind();
		vertexBuffer->Bind();
		m_VertexBuffers.emplace_back(Scope<VertexBuffer>(vertexBuffer));
		SetupLayout(vertexBuffer->GetLayout());
	}

	void OpenGLVertexArray::AddIndexBuffer(IndexBuffer* indexBuffer)
	{
		/*
			We first bind the VAO and then the actual index buffer
			That's what links the VAO with the IndexBuffer in OpenGL.
		*/
		Bind();
		indexBuffer->Bind();
		m_IndexBuffer = Scope<IndexBuffer>(indexBuffer);
	}

	void OpenGLVertexArray::SetupLayout(const VertexLayout& layout) const
	{
		LR_CORE_ASSERT(
			layout.GetElements().size() > 0, 
			"VertexLayout provided was empty!\n"
			"Was the VertexBuffer layout set before adding it to the VertexArray?"
		);

		auto& vertexElements = layout.GetElements();
		for (int i = 0; i < vertexElements.size(); i++)
		{
			VertexElement vertexElement = vertexElements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				vertexElement.TypeInfo.Count,
				GetOpenGLType(vertexElement.TypeInfo.ShaderType),
				vertexElement.Normalized,
				layout.GetStride(),
				reinterpret_cast<void*>(vertexElement.GetOffset())
			);
		}
	}

}
