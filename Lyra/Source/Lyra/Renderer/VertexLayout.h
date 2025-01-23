#pragma once

namespace Lyra
{
	struct VertexTypeInfo
	{
		enum class Type { Float, UInt };

		Type type;
		uint32_t size;

		VertexTypeInfo(Type pType, uint32_t pSize)
			:type(pType), size(pSize) {}

	};

	static const VertexTypeInfo VertexType_Float(VertexTypeInfo::Type::Float, sizeof(float));
	static const VertexTypeInfo VertexType_UInt(VertexTypeInfo::Type::UInt, sizeof(uint32_t));

	struct VertexElement
	{
		uint8_t count;
		VertexTypeInfo typeInfo;
		bool normalized;

		uint32_t GetSize() const { return count * typeInfo.size; }
	};

	class VertexLayout
	{
	public:

		VertexLayout() = default;
		VertexLayout(int count);

		/*
		* Returns the offset in bytes between two consecutive vertex elements.
		*/
		uint32_t GetStride() const { return m_Stride; }
		const std::vector<VertexElement>& GetElements() const { return m_Elements; }

		void AddElement(const VertexTypeInfo& typeInfo, uint8_t count, bool normalized = false);

	private:

		std::vector<VertexElement> m_Elements;
		uint32_t m_Stride = 0;
	};

}