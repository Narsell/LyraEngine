#include "lrpch.h"

#include <glad/glad.h>

#include "OpenGLFrameBuffer.h"
#include "Core/Application.h"

namespace Lyra
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		glCreateFramebuffers(1, &m_RendererId);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTextTarget);
		Resize(spec.width, spec.height);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(1, &m_ColorTextTarget);
	}

	void OpenGLFrameBuffer::Resize(float width, float height)
	{
		m_Specification.width = static_cast<uint16_t>(width);
		m_Specification.height = static_cast<uint16_t>(height);

		if (m_Specification.width <= m_Specification.minWidth)
		{
			m_Specification.width = m_Specification.minWidth;
		}
		if (m_Specification.height <= m_Specification.minHeight)
		{
			m_Specification.height = m_Specification.minHeight;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		// Generate texture object
		glBindTexture(GL_TEXTURE_2D, m_ColorTextTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Specification.width, m_Specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Attach texture to this framebuffer as the color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextTarget, 0);

		// Creating stencil and depth buffers
		unsigned int rbo;
		glCreateRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Attach stencil and depth bufffers as attachments to this framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		LR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation failed.");
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}