#include "lrpch.h"

#include <glad/glad.h>

#include "OpenGLFrameBuffer.h"
#include "Core/Application.h"

namespace Lyra
{
	OpenGLFrameBuffer::OpenGLFrameBuffer()
	{
		glCreateFramebuffers(1, &m_RendererId);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTextTarget);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		const Window& window = Application::Get().GetWindow();
		// Create and bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		// Generate texture object
		glBindTexture(GL_TEXTURE_2D, m_ColorTextTarget);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.GetWidth(), window.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Attach texture to this framebuffer as the color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextTarget, 0);
	
		// Creating stencil and depth buffers
		unsigned int rbo;
		glCreateRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.GetWidth(), window.GetHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Attach stencil and depth bufffers as attachments to this framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


		LR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation failed.");
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFrameBuffer::GetColorTextureTarget() const
	{
		return m_ColorTextTarget;
	}
}