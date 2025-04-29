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
			Type shaderType;
			uint8_t count;
			size_t size;

			TypeInfo(Type inShaderType, uint8_t inCount, size_t inSize)
				: shaderType(inShaderType), count(inCount), size(inSize) {}

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
		const std::string name;
		ShaderData::TypeInfo typeInfo;
		bool normalized;
		size_t offset;

		VertexElement() = delete;
		VertexElement(const std::string& inName, const ShaderData::TypeInfo& inTypeInfo, bool inNormalized = false);

		size_t GetSize() const { return typeInfo.size; }
		size_t GetOffset() const { return offset; }

	};

	class VertexLayout
	{
	public:

		VertexLayout() = default;
		VertexLayout(const std::initializer_list<VertexElement>& elements);

		/*
		* Returns the offset in bytes between two consecutive vertex elements.
		*/
		size_t GetStride() const { return m_Stride; }
		const std::vector<VertexElement>& GetElements() const { return m_Elements; }

		void DebugPrint(const std::string& layoutName);

	private:

		void CalculateStrideAndOffsets();

	private:

		std::vector<VertexElement> m_Elements;
		size_t m_Stride = 0;
	};

}