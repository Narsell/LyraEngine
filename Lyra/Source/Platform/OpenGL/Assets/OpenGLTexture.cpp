#include "lrpch.h"

#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLTexture.h"
#include "Core/Utils.h"

namespace Lyra
{
	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path &texturePath, const TextureProps &textureProps)
		: m_Path(texturePath), m_Props(textureProps)
	{
		LoadTexture();
		// Caching texture slot as an int to avoid a conversion during binding.
		m_CachedTexSlot = Utils::Texture::GetSlotAsInt(m_Props.Slot);
		LR_CORE_ASSERT(Utils::Texture::IsValidTextureSlot(m_CachedTexSlot), "Invalid texture slot!")
		
		// TODO: Take texture props into consideration for this texture
		m_Hash = Utils::Texture::CalculateHash(m_Path);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::Bind() const
	{
		if (Utils::Texture::IsValidTextureSlot(m_CachedTexSlot))
		{
			glBindTextureUnit(m_CachedTexSlot, m_RendererId);
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
		stbi_uc *data = stbi_load(m_Path.string().c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			LR_CORE_FATAL("Failed to load image from path \"{0}\"", m_Path.string());
			return;
		}

		m_Width = width;
		m_Height = height;

		GLTextureFormat textureFormat = Utils::OpenGL::GetTextureFormat(channels, m_Path.c_str());
	
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glBindTexture(GL_TEXTURE_2D, m_RendererId);
		
		// Texture wrapping for text coords outside of [0, 1]
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Utils::OpenGL::GetGLTextureWrap(m_Props.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Utils::OpenGL::GetGLTextureWrap(m_Props.WrapT));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, Utils::OpenGL::GetGLTextureWrap(m_Props.WrapR));
		// If we are using TextureWrap::CLAMP_TO_BORDER in either S or T, we set the border color. Let's not worry about R for now...
		if (m_Props.WrapS == TextureWrap::CLAMP_TO_BORDER || m_Props.WrapT == TextureWrap::CLAMP_TO_BORDER)
		{
			glTextureParameterfv(m_RendererId, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(m_Props.BorderColor));
		}
		// These param control how to handle textures that don't map 1:1 to our geometry. Minification = Shrink, Magnification = Expand.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Utils::OpenGL::GetGLTextureFilter(m_Props.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Utils::OpenGL::GetGLTextureFilter(m_Props.MagFilter));
		
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

}