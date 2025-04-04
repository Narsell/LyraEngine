#pragma once

#include "Core.h"
#include "Core/Log.h"

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
		inline bool IsValidTextureType(int8_t i) { return IsValidTextureType(static_cast<TextureType>(i)); }
		
		/* Retrieves a string representation of a given TextureType. */
		inline const char* TextureTypeToString(TextureType type)
		{
			switch (type)
			{
				case TextureType::DIFFUSE: return "diffuse";
				case TextureType::SPECULAR: return "specular";
				default:
				{
					LR_CORE_ERROR("Invalid TextureType: {0}!", static_cast<int>(type));
					return "invalid";
				}
			}
		}
	}

	namespace Utils::Hash
	{
		/* Stolen from stackoverflow, thanks @Matteo Italia */
		template <class T>
		inline void HashCombine(std::size_t& seed, const T& v)
		{
			std::hash<T> hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		template <typename T, typename... Rest>
		inline void HashCombine(std::size_t& seed, const T& v, Rest... rest) {
			std::hash<T> hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			HashCombine(seed, rest...);
		}
	}

}