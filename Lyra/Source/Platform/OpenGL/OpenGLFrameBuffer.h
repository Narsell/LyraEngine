#pragma once

#include <cstdint>
#include "Renderer/FrameBuffer.h"

namespace Lyra
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer();

		virtual void Resize(uint16_t width, uint16_t height) override;
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetColorTextureTarget() const override { return m_ColorTextTarget; }

	private:
		uint32_t m_RendererId;
		uint32_t m_ColorTextTarget;
		FrameBufferSpecification m_Specification;
	};
}
