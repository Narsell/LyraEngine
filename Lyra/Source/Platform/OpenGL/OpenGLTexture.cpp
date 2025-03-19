#include "lrpch.h"

#include <stb_image.h>

#include "OpenGLTexture.h"
#include "Core/Utils.h"

namespace Lyra
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, TextureType textureType)
		:	Texture2D(textureType), m_Height(0.0f), m_Width(0.0f), m_RendererId(0), m_Path(path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			LR_CORE_FATAL("Failed to load image from path \"{0}\"", path.c_str());
			return;
		}

		m_Width = width;
		m_Height = height;

		GLTextureFormat textureFormat = GetTextureFormat(channels);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		// Allocates memory in the GPU for the texture data that we want to upload.
		glTextureStorage2D(m_RendererId, 1, textureFormat.Internal, m_Width, m_Height);

		// These param control how to handle textures that don't map 1:1 to our geometry. Minification = Shrink, Magnification = Expand.
		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Upload data to GPU
		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, textureFormat.Data, GL_UNSIGNED_BYTE, data);

		// Free texture data from CPU since we don't need it once it has been uploaded to the GPU.
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::Bind() const
	{
		if (Utils::Texture::IsValidTextureType(m_Type))
		{
			glBindTextureUnit(m_Slot, m_RendererId);
		}
	}

	GLTextureFormat OpenGLTexture2D::GetTextureFormat(int channels) const
	{
		GLTextureFormat textureFormat;
		switch (channels)
		{
			case 1:
			{
				textureFormat.Internal = GL_R8;
				textureFormat.Data = GL_RED;
				break;
			}
			case 3:
			{
				textureFormat.Internal = GL_RGB8;
				textureFormat.Data = GL_RGB;
				break;
			}
			case 4:
			{
				textureFormat.Internal = GL_RGBA8;
				textureFormat.Data = GL_RGBA;
				break;
			}
			default:
			{
				LR_CORE_ASSERT(false, "Texture at '{}': Format not supported.", m_Path);
			}
		}
		return textureFormat;
	}
}