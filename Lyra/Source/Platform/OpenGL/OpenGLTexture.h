#pragma once

#include "Scene/Texture.h"

#include <glad/glad.h>
#include <string>

namespace Lyra
{
	struct GLTextureFormat
	{
		// The format Open GL will use to allocate memory in the GPU for the texture.
		GLenum Internal;
		// The format of the texture data to upload.
		GLenum Data;
	};

	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(const Texture2DProps& textureProps);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return  m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererId() const { return m_RendererId; }
		virtual size_t GetHash() const override { return m_Hash; }

		virtual void Bind() const override;

	private:
		GLTextureFormat GetTextureFormat(int channels) const;

	private:
	
		uint32_t m_Height = 0;
		uint32_t m_Width = 0;
		const uint8_t Slot;

		uint32_t m_RendererId = 0;
		size_t m_Hash = 0;
	};

}