#include "lrpch.h"

#include "VertexLayout.h"

namespace Lyra
{
	/* ############################# */
	/* ####   VERTEX ELEMENT	#### */
	/* ############################# */

	VertexElement::VertexElement(const std::string& name, const ShaderData::TypeInfo& typeInfo, bool normalized)
		: Name(name), TypeInfo(typeInfo), Normalized(normalized), Offset(0)
	{
	}


	/* ############################# */
	/* ####   VERTEX LAYOUT 	#### */
	/* ############################# */

	VertexLayout::VertexLayout(const std::initializer_list<VertexElement>& elements)
		: m_Elements(elements)
	{
		CalculateStrideAndOffsets();
	}

	void VertexLayout::CalculateStrideAndOffsets()
	{
		m_Stride = 0;
		uint32_t offset = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.GetSize();
		}
		m_Stride = offset;
	}

	void VertexLayout::DebugPrint(const std::string& layoutName)
	{
		LR_CORE_INFO("[{0}] Vertex Buffer Layout", layoutName);
		for (auto& element : m_Elements)
		{
			LR_CORE_INFO("  {0}", element.Name);
			LR_CORE_INFO("    Type: {0}", ShaderTypeAsString(element.TypeInfo.ShaderType));
			LR_CORE_INFO("    Size: {0}", element.GetSize());
			LR_CORE_INFO("    Offset: {0}", element.GetOffset());
		}
		LR_CORE_INFO("  Total Stride: {0}", GetStride());
	}
}