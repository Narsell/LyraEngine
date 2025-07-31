#include "lrpch.h"

#include "Mesh.h"
#include "Core/Utils.h"
#include "Renderer/Renderer.h"

namespace Lyra
{
    Mesh::Mesh(const std::string &name, const VertexArray* vertexArray, const Ref<Material> &material)
		:m_Name(name), m_Material(material)
	{
		m_VertexArray = Scope<const VertexArray>(vertexArray);
		m_VertexCount = m_VertexArray->GetVertexBuffers().at(0)->GetVertexCount();
		m_Hash = Utils::Mesh::CalculateHash(m_VertexArray->GetRendererId(), 
											m_VertexArray->GetIndexBuffer()->GetRendererId(),
											m_Material->GetHash());
    }

    void Mesh::Draw(const glm::mat4& transform, RenderType renderType) const
	{
		Renderer::Submit(m_Material, m_VertexArray, transform, true, renderType);
	}
}