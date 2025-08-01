#include "lrpch.h"

#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLCubemap.h"
#include "Platform/OpenGL/Utils/OpenGLUtils.h"
#include "Core/Utils.h"

namespace Lyra
{
    OpenGLCubemap::OpenGLCubemap(const std::vector<std::filesystem::path>& texturePaths, const TextureProps& textureProps)
        : m_Paths(texturePaths), m_Props(textureProps), m_DirectoryPath(texturePaths.front().parent_path())
    {
        m_Hash = Utils::Texture::CalculateListHash(texturePaths, textureProps);
    
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);

        for (int i = 0; i < texturePaths.size(); ++i)
        {
            auto& path = texturePaths[i];

            if (!std::filesystem::exists(path))
            {
                LR_CORE_FATAL("Path to texture doesn't exist... '{0}'", path.string().c_str());
                return;
            }

            int width, height, channels;
            stbi_set_flip_vertically_on_load(static_cast<int>(m_Props.FlipVertically));
            stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
            if (!data)
            {
                LR_CORE_FATAL("Failed to load image from path \"{0}\"", path.string().c_str());
                return;
            }
            GLTextureFormat textureFormat = Utils::OpenGL::GetTextureFormat(channels, path.string().c_str());
            const uint32_t textureTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        
			glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            
            // Texture wrapping for text coords outside of [0, 1]
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Utils::OpenGL::GetGLTextureWrap(m_Props.WrapS));
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Utils::OpenGL::GetGLTextureWrap(m_Props.WrapT));
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Utils::OpenGL::GetGLTextureWrap(m_Props.WrapR));
            // If we are using TextureWrap::CLAMP_TO_BORDER in either S or T, we set the border color. Let's not worry about R for now...
            if (m_Props.WrapS == TextureWrap::CLAMP_TO_BORDER || m_Props.WrapT == TextureWrap::CLAMP_TO_BORDER)
            {
                glTextureParameterfv(m_RendererId, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(m_Props.BorderColor));
            }
            // These param control how to handle textures that don't map 1:1 to our geometry. Minification = Shrink, Magnification = Expand.
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Utils::OpenGL::GetGLTextureFilter(m_Props.MinFilter));
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Utils::OpenGL::GetGLTextureFilter(m_Props.MagFilter));
            
            if (m_Props.UseMipmaps)
            {
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            }
        }
    }

    void OpenGLCubemap::Bind() const
    {
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);
    }
}