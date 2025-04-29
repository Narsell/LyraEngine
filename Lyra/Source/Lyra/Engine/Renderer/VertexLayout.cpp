#include "lrpch.h"

#include "VertexLayout.h"

namespace Lyra
{
	/* ############################# */
	/* ####   VERTEX ELEMENT	#### */
	/* ############################# */

	VertexElement::VertexElement(const std::string& inName, const ShaderData::TypeInfo& inTypeInfo, bool inNormalized)
		: name(inName), typeInfo(inTypeInfo), normalized(inNormalized), offset(0)
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
		size_t offset = 0;
		for (auto& element : m_Elements)
		{
			element.offset = offset;
			offset += element.GetSize();
		}
		m_Stride = offset;
	}

	void VertexLayout::DebugPrint(const std::string& layoutName)
	{
		LR_CORE_INFO("[{0}] Vertex Buffer Layout", layoutName);
		for (auto& element : m_Elements)
		{
			LR_CORE_INFO("  {0}", element.name);
			LR_CORE_INFO("    Type: {0}", ShaderTypeAsString(element.typeInfo.shaderType));
			LR_CORE_INFO("    Size: {0}", element.GetSize());
			LR_CORE_INFO("    Offset: {0}", element.GetOffset());
		}
		LR_CORE_INFO("  Total Stride: {0}", GetStride());
	}
}