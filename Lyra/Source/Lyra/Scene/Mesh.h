#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Core.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Scene/Material.h"

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
        Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const Ref<Material>& material = std::make_shared<Material>());
        void Draw(const glm::mat4& transform);

        const Ref<Material>& GetMaterial() const { return m_Material; }
        uint64_t GetVertexCount() const { return m_VertexCount; }
        const std::string_view GetName() const { return std::string_view(m_Name.c_str(), m_Name.size()); }
        size_t GetHash() const { return m_Hash; }
    private:

        Scope<VertexArray> m_VertexArray;
        Ref<Material> m_Material;

        const std::string m_Name;
        uint64_t m_VertexCount;

        size_t m_Hash = 0;
    };
}
