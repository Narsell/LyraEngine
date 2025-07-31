#pragma once

#include <filesystem>
#include <vector>

#include "Core/Core.h"
#include "Assets/Texture.h"

namespace Lyra
{
    class Mesh; 
    class Material;
    class VertexArray;

    LYRA_API class Skybox
    {
    public:
        Skybox(const std::vector<std::filesystem::path>& paths);

        void Draw() const;
    private:
        const VertexArray* CreateVertexData() const;    

    private:
        uint32_t m_RendererId = 0;
        TextureProps m_CubemapProps;
        Ref<Material> m_Material;
        Ref<CubemapTexture> m_CubemapTexture;
        Ref<Mesh> m_Mesh;
    };
}