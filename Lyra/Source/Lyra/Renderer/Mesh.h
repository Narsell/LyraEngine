#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Lyra/Core/Core.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Buffer.h"
#include "Lyra/Material.h"

/* This will all be OpenGL specific and API agnostic later */
namespace Lyra
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord = glm::vec2(0.0f, 0.0f);
    };

    class LYRA_API Mesh {
    public:
        Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const Ref<Material>& material);
        void Draw();
    private:

        /* TODO: Convert these to Scope (unique_ptr) */
        Ref<VertexArray> m_VertexArray;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Material> m_Material;

        const std::string m_Name;
    };
}
