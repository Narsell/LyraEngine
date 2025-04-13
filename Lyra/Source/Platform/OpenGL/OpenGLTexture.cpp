#include "lrpch.h"

#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLTexture.h"
#include "Core/Utils.h"

namespace Lyra
{
	constexpr int GetGLTextureFilter(TextureFilter filter)
	{
		switch (filter)
		{
			case TextureFilter::NEAREST:				return GL_NEAREST;
			case TextureFilter::LINEAR:					return GL_LINEAR;
			case TextureFilter::NEAREST_MIPMAP_NEAREST:	return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilter::LINEAR_MIPMAP_NEAREST:	return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilter::NEAREST_MIPMAP_LINEAR:	return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilter::LINEAR_MIPMAP_LINEAR:	return GL_LINEAR_MIPMAP_LINEAR;
			default:
			{
				LR_CORE_ERROR("Invalid TextureFilter: {0}!", static_cast<int>(filter));
				return GL_LINEAR;
			}
		}
	}

	constexpr int GetGLTextureWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::REPEAT:			return GL_REPEAT;
			case TextureWrap::MIRRORED_REPEAT:	return GL_MIRRORED_REPEAT;
			case TextureWrap::CLAMP_TO_EDGE:	return GL_CLAMP_TO_EDGE;
			case TextureWrap::CLAMP_TO_BORDER:	return GL_CLAMP_TO_BORDER;
			default:
			{
				LR_CORE_ERROR("Invalid TextureWrap: {0}!", static_cast<int>(wrap));
				return GL_REPEAT;
			}
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& texturePath, const Texture2DProps& textureProps)
		:	Texture2D(texturePath, textureProps), Slot(static_cast<int8_t>(textureProps.Type))
	{
		LoadTexture();
		// TODO: Take texture props into consideration for this texture
		m_Hash = Utils::Texture::CalculateHash(m_Path.string());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::Bind() const
	{
		if (Utils::Texture::IsValidTextureType(m_Props.Type))
		{
			glBindTextureUnit(Slot, m_RendererId);
		}
	}

	void OpenGLTexture2D::LoadTexture()
	{
		if (!std::filesystem::exists(m_Path))
		{
			LR_CORE_FATAL("Path to texture doesn't exist... '{0}'", m_Path.string());
			return;
		}

		int width, height, channels;
		stbi_set_flip_vertically_on_load(static_cast<int>(m_Props.FlipVertically));
		stbi_uc* data = stbi_load(m_Path.string().c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			LR_CORE_FATAL("Failed to load image from path \"{0}\"", m_Path.string());
			return;
		}

		m_Width = width;
		m_Height = height;

		GLTextureFormat textureFormat = GetTextureFormat(channels);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glBindTexture(GL_TEXTURE_2D, m_RendererId);

		// Texture wrapping for text coords outside of [0, 1]
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLTextureWrap(m_Props.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLTextureWrap(m_Props.WrapT));
		// If we are using TextureWrap::CLAMP_TO_BORDER in either S or T, we set the border color.
		if (m_Props.WrapS == TextureWrap::CLAMP_TO_BORDER || m_Props.WrapT == TextureWrap::CLAMP_TO_BORDER)
		{
			glTextureParameterfv(m_RendererId, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(m_Props.BorderColor));
		}
		// These param control how to handle textures that don't map 1:1 to our geometry. Minification = Shrink, Magnification = Expand.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLTextureFilter(m_Props.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLTextureFilter(m_Props.MagFilter));

		// Allocates memory in the GPU for the texture data that we want to upload.
		glTextureStorage2D(m_RendererId, 1, textureFormat.Internal, m_Width, m_Height);

		// Upload data to GPU
		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, textureFormat.Data, GL_UNSIGNED_BYTE, data);

		if (m_Props.UseMipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Free texture data from CPU since we don't need it once it has been uploaded to the GPU.
		stbi_image_free(data);
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
				LR_CORE_ASSERT(false, "Texture at '{}': Format not supported.", m_Path.string());
			}
		}
		return textureFormat;
	}
}