#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Core.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Assets/Material.h"

namespace Lyra
{
    struct Vertex
    {
        glm::vec3 Position = glm::vec3();
        glm::vec3 Normal = glm::vec3();
        glm::vec2 TexCoord = glm::vec2(0.0f, 0.0f);
    };

    class LYRA_API Mesh {
    public:
        Mesh(const std::string& name, const VertexArray* vertexArray, const Ref<Material>& material = std::make_shared<Material>());
        void Draw(const glm::mat4& transform, RenderType renderType = RenderType::LR_OPAQUE) const;

        const Ref<Material>& GetMaterial() const { return m_Material; }
        uint64_t GetVertexCount() const { return m_VertexCount; }
        const std::string_view GetName() const { return std::string_view(m_Name.c_str(), m_Name.size()); }
        size_t GetHash() const { return m_Hash; }
    private:

        Scope<const VertexArray> m_VertexArray;
        Ref<Material> m_Material;

        const std::string m_Name;
        uint64_t m_VertexCount;

        size_t m_Hash = 0;
    };
}
