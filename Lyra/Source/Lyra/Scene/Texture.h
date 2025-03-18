#pragma once

#include "Core/Ref.h"

/* TEXTURE TYPE ENUM AND UTILS */
namespace Lyra
{
	enum class TextureType : int8_t
	{
		NONE = -1,
		DIFFUSE,
		SPECULAR,

		COUNT = SPECULAR + 1
	};

	namespace Utils::Texture
	{
		/* Gets the number of unique TextureType elements supported by the engine. */
		inline constexpr int8_t GetUniqueTypeCount() { return static_cast<int8_t>(TextureType::COUNT); }
		/* Checks if the given TextureType is valid. */
		inline bool IsValidTextureType(TextureType type)
		{
			int8_t typeAsInt = static_cast<int8_t>(type);
			return typeAsInt > static_cast<int8_t>(TextureType::NONE)
				&& typeAsInt < static_cast<int8_t>(TextureType::COUNT);
		}
		/* Validates if a given integer maps to a valid TextureType. */
		inline bool IsValidTextureType(int8_t i)
		{
			return IsValidTextureType(static_cast<TextureType>(i));
		}
		/* Gets the slot number that the given TextureType binds to. */
		inline constexpr int8_t GetTypeSlot(TextureType type)
		{
			return static_cast<int8_t>(type);
		}
		/* Retrieves a string representation of a given TextureType. */
		inline const char* TextureTypeToString(TextureType type)
		{
			switch (type)
			{
			case Lyra::TextureType::DIFFUSE: return "diffuse";
			case Lyra::TextureType::SPECULAR: return "specular";
			default:
			{
				LR_CORE_ERROR("Invalid TextureType: {0}!", static_cast<int>(type));
				return "invalid";
			}
			}
		}
	}
}

/* TEXTURE INTERFACE */
namespace Lyra
{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetRendererId() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::string& GetPath() const = 0;

		virtual void Bind() const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		TextureType GetType() const { return m_Type; }
		const char* GetTypeAsString() const { return Utils::Texture::TextureTypeToString(m_Type); }

		/* Keeping count of loaded textures here... TODO: Move this to an asset manager eventually! */
		static std::unordered_map<std::string, Ref<Texture2D>> s_TexturesLoaded;
		static Ref<Texture2D> Create(const std::string& path, TextureType textureType = TextureType::NONE);

	protected:
		Texture2D(TextureType textureType = TextureType::NONE) 
			:	m_Type(textureType), 
				m_Slot(Utils::Texture::GetTypeSlot(m_Type))
		{
			if (!Utils::Texture::IsValidTextureType(m_Type))
			{
				LR_CORE_WARN("Texture at path '{0}' was not given a valid type. Won't bind this texture.");
			}
		}

	protected:
		const TextureType m_Type = TextureType::NONE;
		const uint8_t m_Slot;
	};
}
