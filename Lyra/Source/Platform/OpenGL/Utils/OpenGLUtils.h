#pragma once

#include <glad/glad.h>

#include "Core.h"
#include "Assets/Texture.h"

namespace Lyra
{
    struct GLTextureFormat
	{
		// The format Open GL will use to allocate memory in the GPU for the texture.
		GLenum Internal;
		// The format of the texture data to upload.
		GLenum Data;
	};

	namespace Utils::OpenGL
	{
		inline constexpr GLTextureFormat GetTextureFormat(int channels, const char* path)
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
				LR_CORE_ASSERT(false, "Format not supported for texture at '{}'", path);
			}
			}
			return textureFormat;
		}

		inline constexpr int GetGLTextureFilter(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::NEAREST:						return GL_NEAREST;
			case TextureFilter::LINEAR:							return GL_LINEAR;
			case TextureFilter::NEAREST_MIPMAP_NEAREST:			return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilter::LINEAR_MIPMAP_NEAREST:			return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilter::NEAREST_MIPMAP_LINEAR:			return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilter::LINEAR_MIPMAP_LINEAR:			return GL_LINEAR_MIPMAP_LINEAR;
			default:
			{
				LR_CORE_ERROR("Invalid TextureFilter: {0}!", static_cast<int>(filter));
				return GL_LINEAR;
			}
			}
		}

		inline constexpr int GetGLTextureWrap(TextureWrap wrap)
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
	}
}