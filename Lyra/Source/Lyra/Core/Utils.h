#pragma once

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

	namespace Utils
	{
		namespace Hash
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

		namespace Texture
		{
			inline size_t CalculateHash(const std::string& path)
			{
				size_t textureHash = 0;
				Hash::HashCombine(textureHash, path);
				return textureHash;
			}
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

		namespace Material
		{
			// Shininess temp is temporary... TODO: Have some sort of material properties struct that holds that data.
			inline size_t CalculateHash(size_t shaderHash, size_t materialListHash, float shininess)
			{
				size_t materialHash = 0;
				Hash::HashCombine(materialHash, shaderHash, materialListHash, shininess);
				return materialHash;
			}
		}

		namespace Mesh
		{
			inline size_t CalculateHash(uint32_t vaId, uint32_t ibId, size_t materialHash)
			{
				size_t meshHash = 0;
				Hash::HashCombine(meshHash, (size_t)vaId, (size_t)ibId, materialHash);
				return meshHash;
			}
		}

		namespace Model
		{
			inline size_t CalculateHash(const std::string& path, float textureFlipOverride)
			{
				size_t modelHash = 0;
				// TODO: Gotta figure out a better way to integrate props into hash calculation!
				Hash::HashCombine(modelHash, path, textureFlipOverride);
				return modelHash;
			}
		}
	}
}