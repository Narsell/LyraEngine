#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Lyra/Core/Core.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Buffer.h"
#include "Lyra/Renderer/Texture.h"

/* This will all be OpenGL specific and API agnostic later */
namespace Lyra
{
    enum class TextureType : uint8_t
    {
        NONE = 0,
        DIFFUSE,
        SPECULAR
    };

    struct MeshTexture2D
    {
        Ref<Texture2D> Texture;
        TextureType Type = TextureType::NONE;
    };

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord = glm::vec2(0.0f, 0.0f);
    };

    class LYRA_API Mesh {
    public:
        Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::vector<MeshTexture2D>& textures);
        void Draw(Ref<Shader> shader);
    private:

        /* TODO: Convert these to Scope (unique_ptr) */
        Ref<VertexArray> m_VertexArray;
        Ref<IndexBuffer> m_IndexBuffer;
        const std::vector<MeshTexture2D> m_Textures;

        const std::string m_Name;
    };
}
