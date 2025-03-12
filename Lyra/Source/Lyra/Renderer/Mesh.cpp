#include "lrpch.h"

#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Lyra/Renderer/Renderer.h"

namespace Lyra
{

	Mesh::Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::vector<MeshTexture2D>& textures)
		:	m_Name(name), m_Textures(textures)
	{
		m_VertexArray = Ref<VertexArray>(VertexArray::Create());

		VertexLayout layout
		{
			{ "a_Position", ShaderData::Float3},
			{ "a_Normal",	ShaderData::Float3},
			{ "a_TexCoord", ShaderData::Float2},
		};

		Ref<VertexBuffer> meshVertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create(glm::value_ptr(vertices[0].Position), vertices.size() * sizeof(vertices[0]), layout));
		m_VertexArray->AddVertexBuffer(meshVertexBuffer);
		
		m_IndexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(&indices[0], indices.size()));
		m_VertexArray->AddIndexBuffer(m_IndexBuffer);
	}

	void Mesh::Draw(Ref<Shader> shader)
	{
		unsigned int diffuseN = 1;
		unsigned int specularN = 1;

		for (int i = 0; i < m_Textures.size(); i++)
		{
			std::string propertyName;
			TextureType textureType = m_Textures[i].Type;

			switch (textureType)
			{
				case Lyra::TextureType::NONE:
				{
					LR_CORE_FATAL("Texture at index {0} in mesh {1} doesn't have a type. Skipping draw call.", i, m_Name);
					return;
				}
				case Lyra::TextureType::DIFFUSE:
				{
					propertyName = std::format("diffuse{0}", diffuseN++);
					break;
				}
				case Lyra::TextureType::SPECULAR:
				{
					propertyName = std::format("specular{0}", specularN++);
					break;
				}
			}

			shader->UploadUniform_1i(std::format("u_Material.{0}", propertyName), i);
		}

		Renderer::Submit(shader, m_VertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	}

}