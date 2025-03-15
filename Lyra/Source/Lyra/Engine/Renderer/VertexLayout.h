#pragma once

namespace Lyra
{
	namespace ShaderData
	{
		/* Defines the types that can be used to describe vertex elements in a vertex layout.  */
		enum class Type
		{
			Float, Float2, Float3, Float4, Int, UInt
		};

		/* TypeInfo contains the metadata for a given ShaderData::Type */
		struct TypeInfo
		{
			Type ShaderType;
			uint8_t Count;
			uint32_t Size;

			TypeInfo(Type shaderType, uint8_t count, uint32_t size)
				: ShaderType(shaderType), Count(count), Size(size) {}

		};

		/* STATIC SHADER TYPE INFO																		*/
		/* This is where you can declare the metadata of a given ShaderData::Type						*/
		/* e.g: A Type::Float2 has 2 components (Count) and a total size (Size) of 2 * sizeof(float)	*/
		/*| Type declaration		  |	Enum Type	 | N. of components |	Complete size     |			*/
		static const TypeInfo Float    (Type::Float,		 1,			 	 sizeof(float));
		static const TypeInfo Float2   (Type::Float2,		 2,			 2 * sizeof(float));
		static const TypeInfo Float3   (Type::Float3,		 3,			 3 * sizeof(float));
		static const TypeInfo Float4   (Type::Float4,		 4,			 4 * sizeof(float));
		static const TypeInfo Int      (Type::Int,			 1,			 	 sizeof(int));
		static const TypeInfo UInt     (Type::UInt,			 1,			 	 sizeof(uint32_t));

		/* DEBUG STUFF */

		inline std::string ShaderTypeAsString(const ShaderData::Type& value)
		{
			#define PROCESS_VAL(p) case(p): return std::string((#p)).erase(0, 6);
			switch (value) {
				PROCESS_VAL(Type::Float);
				PROCESS_VAL(Type::Float2);
				PROCESS_VAL(Type::Float3);
				PROCESS_VAL(Type::Float4);
				PROCESS_VAL(Type::Int);
				PROCESS_VAL(Type::UInt);
			}
			#undef PROCESS_VAL
			return "TYPE_NOT_MAPPED_FOR_DEBUG";
		}
	}

	struct VertexElement
	{
		std::string Name;
		ShaderData::TypeInfo TypeInfo;
		bool Normalized;
		uint32_t Offset;

		VertexElement() = delete;
		VertexElement(const std::string& name, const ShaderData::TypeInfo& typeInfo, bool normalized = false);

		uint32_t GetSize() const { return TypeInfo.Size; }
		uint32_t GetOffset() const { return Offset; }

	};

	class VertexLayout
	{
	public:

		VertexLayout() = default;
		VertexLayout(const std::initializer_list<VertexElement>& elements);

		/*
		* Returns the offset in bytes between two consecutive vertex elements.
		*/
		uint32_t GetStride() const { return m_Stride; }
		const std::vector<VertexElement>& GetElements() const { return m_Elements; }

		void DebugPrint(const std::string& layoutName);

	private:

		void CalculateStrideAndOffsets();

	private:

		std::vector<VertexElement> m_Elements;
		uint32_t m_Stride = 0;
	};

}