#pragma once

namespace Lyra
{
	struct FrameBufferSpecification
	{
		uint16_t width, height;

		uint16_t minWidth, minHeight = 100;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Resize(float width, float height) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorTextureTarget() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}
