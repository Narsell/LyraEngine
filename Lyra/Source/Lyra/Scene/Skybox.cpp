#include "lrpch.h"

#include "Skybox.h"

#include <vector>

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"
#include "Assets/TextureLibrary.h"
#include "Assets/MaterialLibrary.h"
#include "Assets/Mesh.h"

namespace Lyra
{
    Skybox::Skybox(const std::vector<std::filesystem::path>& paths)
    { 
        // Loading cubemap textures
        m_CubemapProps.UseMipmaps = false;
        m_CubemapProps.MinFilter = TextureFilter::LINEAR;
        m_CubemapProps.MagFilter = TextureFilter::LINEAR;
        m_CubemapProps.WrapS = TextureWrap::CLAMP_TO_EDGE;
        m_CubemapProps.WrapT = TextureWrap::CLAMP_TO_EDGE;
        m_CubemapProps.WrapR = TextureWrap::CLAMP_TO_EDGE;
      
        m_CubemapTexture = TextureLibrary::LoadCubemapTexture(paths, m_CubemapProps);
      
        // Creating material
        Ref<Shader> shader = ShaderLibrary::Load("Assets/Shaders/Skybox.glsl");
        shader->SetHasLightData(false);
        m_Material = MaterialLibrary::Create(shader, { m_CubemapTexture });

        // Vertex array with vertex buffer and indices
        const VertexArray* vertexArray = CreateVertexData();

        m_Mesh = std::make_shared<Mesh>("Skybox", vertexArray, m_Material);
    }

    void Skybox::Draw() const
    {
        // Note that the transform submitted here does nothing.
        // The Skybox's transform is calculated during runtime to adjust for the camera's clipping bounds.
        // This is done so it's always at the 'edge of the world'.
        m_Mesh->Draw(glm::mat4(1.0f), RenderType::LR_SKYBOX);
    }

    const VertexArray* Skybox::CreateVertexData() const
    {
        constexpr float vertices[] = {
            -1.0, -1.0, -1.0, // Back face
             1.0,  1.0, -1.0, 
            -1.0,  1.0, -1.0,
             1.0, -1.0, -1.0,
             
            -1.0, -1.0,  1.0, // Front face
             1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
             1.0, -1.0,  1.0,
             
            -1.0, -1.0,  1.0, // Left face
            -1.0,  1.0, -1.0,
            -1.0,  1.0,  1.0, 
            -1.0, -1.0, -1.0, 
            
             1.0, -1.0, -1.0, // Right face
             1.0,  1.0,  1.0, 
             1.0,  1.0, -1.0,
             1.0, -1.0,  1.0, 
        
            -1.0,  1.0,  1.0, // Top face
             1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
             1.0,  1.0,  1.0,
        
            -1.0, -1.0,  1.0, // Bottom face
             1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0,
             1.0, -1.0,  1.0,
        };

        constexpr uint32_t indices [] = {
             0,  1,  2,  0,  3,  1,     // Back
             4,  6,  5,  4,  5,  7,     // Front
             8,  9, 10,  8, 11,  9,     // Left
            12, 13, 14, 12, 15, 13,     // Right
            16, 18, 17, 16, 17, 19,     // Top
            20, 21, 22, 20, 23, 21,     // Bottom
        };
        
        const VertexLayout vertexLayout
        {
            {"a_Position", ShaderData::Float3}
        };

        VertexArray* vertexArray = VertexArray::Create();
        VertexBuffer* vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices) * sizeof(vertices[0]), vertexLayout);
        vertexArray->AddVertexBuffer(vertexBuffer);
        IndexBuffer* indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(indices[0]));
        vertexArray->AddIndexBuffer(indexBuffer); 

        return vertexArray;
    }
}