#pragma once

#include <cstdint>
#include "Renderer/FrameBuffer.h"

namespace Lyra
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetColorTextureTarget() const override;

	private:
		uint32_t m_RendererId;
		uint32_t m_ColorTextTarget;
	};
}
