#include "lrpch.h"

#include "VertexLayout.h"

namespace Lyra
{
	VertexLayout::VertexLayout(int count)
	{
		m_Elements.reserve(count);
	}

	void VertexLayout::AddElement(const VertexTypeInfo& typeInfo, uint8_t count, bool normalized)
	{
		m_Elements.emplace_back(count, typeInfo, normalized);
		m_Stride += m_Elements.back().GetSize();
	}

}