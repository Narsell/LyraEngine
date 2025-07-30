#include "lrpch.h"

#include "Skybox.h"

#include <vector>

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"
#include "Assets/TextureLibrary.h"
#include "Assets/MaterialLibrary.h"

namespace Lyra
{
    Skybox::Skybox(const std::vector<std::filesystem::path>& paths)
        : m_VertexArray(VertexArray::Create())
    {
        // TODO: Use mesh API to load vertex and material instead
        // Also so we can draw this indexed
        float vertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
 
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        
        VertexLayout vertexLayout
        {
            {"a_Position", ShaderData::Float3}
        };

        VertexBuffer* vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), vertexLayout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);
         
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
    }

    void Skybox::Draw() const
    {
        // Note that the transform submitted here does nothing.
        // Skybox's transform is calculated during runtime to adjust for the camera's clipping bounds.
        // This is done so it's always at the 'edge of the world'.
	    Renderer::Submit(m_Material, m_VertexArray, glm::mat4(1.0), false, RenderType::LR_SKYBOX);
    }
}