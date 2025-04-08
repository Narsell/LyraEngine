#include "lrpch.h"

#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Core/Utils.h"
#include "Renderer/Renderer.h"

namespace Lyra
{

	Mesh::Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const Ref<Material>& material)
		:	m_Name(name), m_Material(material)
	{
		m_VertexArray = Scope<VertexArray>(VertexArray::Create());

		VertexLayout layout
		{
			{ "a_Position", ShaderData::Float3},
			{ "a_Normal",	ShaderData::Float3},
			{ "a_TexCoord", ShaderData::Float2},
		};

		VertexBuffer* meshVertexBuffer = VertexBuffer::Create(glm::value_ptr(vertices[0].Position), vertices.size() * sizeof(vertices[0]), layout);
		m_VertexArray->AddVertexBuffer(meshVertexBuffer);
		
		IndexBuffer* indexBuffer = IndexBuffer::Create(&indices[0], indices.size());
		m_VertexArray->AddIndexBuffer(indexBuffer);

		m_VertexCount = vertices.size();

		m_Hash = Utils::Mesh::CalculateHash(m_VertexArray->GetRendererId(), indexBuffer->GetRendererId(), m_Material->GetHash());
	}

	void Mesh::Draw(const glm::mat4& transform)
	{
		Renderer::Submit(m_Material, m_VertexArray, transform);
	}
}