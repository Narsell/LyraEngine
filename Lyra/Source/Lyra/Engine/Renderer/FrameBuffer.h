#pragma once

namespace Lyra
{
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetColorTextureTarget() const = 0;

		static Ref<FrameBuffer> Create();
	};
}
