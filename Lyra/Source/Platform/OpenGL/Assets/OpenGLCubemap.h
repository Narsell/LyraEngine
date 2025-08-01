#pragma once

#include <vector>
#include <filesystem>

#include "Core.h"
#include "Assets/Texture.h"

namespace Lyra
{
    class OpenGLCubemap final : public CubemapTexture
    {
    public:
        OpenGLCubemap(const std::vector<std::filesystem::path>& texturePaths, const TextureProps& textureProps);

        virtual uint32_t GetRendererId() const override { return m_RendererId; }
        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual const TextureProps& GetProps() const override { return m_Props; }
        virtual const std::filesystem::path& GetPath() const override { return m_DirectoryPath; }
        virtual size_t GetHash() const override { return m_Hash; }
        virtual void Bind() const override;

    private:
        uint32_t m_RendererId = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        TextureProps m_Props;
        const std::filesystem::path m_DirectoryPath;
        const std::vector<std::filesystem::path> m_Paths;
        size_t m_Hash = 0;
    };
}