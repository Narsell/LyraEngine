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

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, TextureType textureType = TextureType::NONE);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetRendererId() const { return m_RendererId; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const std::string& GetPath() const { return m_Path; }


		virtual void Bind() const override;

	private:
		GLTextureFormat GetTextureFormat(int channels) const;

	private:
		std::string m_Path;

		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;
	};

}