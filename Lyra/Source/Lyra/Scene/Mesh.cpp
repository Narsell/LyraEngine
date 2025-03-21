#include "lrpch.h"

#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Renderer/Renderer.h"

namespace Lyra
{

	Mesh::Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const Ref<Material>& material)
		:	m_Name(name), m_Material(material)
	{
		m_VertexArray = Ref<VertexArray>(VertexArray::Create());

		VertexLayout layout
		{
			{ "a_Position", ShaderData::Float3},
			{ "a_Normal",	ShaderData::Float3},
			{ "a_TexCoord", ShaderData::Float2},
		};

		Ref<VertexBuffer> meshVertexBuffer = VertexBuffer::Create(glm::value_ptr(vertices[0].Position), vertices.size() * sizeof(vertices[0]), layout);
		m_VertexArray->AddVertexBuffer(meshVertexBuffer);
		
		m_IndexBuffer = IndexBuffer::Create(&indices[0], indices.size());
		m_VertexArray->AddIndexBuffer(m_IndexBuffer);
	}

	void Mesh::Draw()
	{
		m_Material->UpdateData();
		Renderer::Submit(m_Material->GetShader(), m_VertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)));
	}

}