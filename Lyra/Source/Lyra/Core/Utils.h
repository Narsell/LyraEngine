#pragma once

#include <vector>
#include <filesystem>

#include "Core/Log.h"
#include "Assets/Texture.h"

namespace Lyra
{
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
			inline size_t CalculateHash(const std::filesystem::path& path)
			{
				size_t textureHash = 0;
				Hash::HashCombine(textureHash, path.string());
				return textureHash;
			}

			inline size_t CalculateListHash(const std::vector<std::filesystem::path>& paths)
			{
				size_t combinedHash = 0;
				for (const auto& path : paths)
				{
					Hash::HashCombine(combinedHash, path.string());
				}
				return combinedHash;
			}

			inline constexpr int8_t GetSlotAsInt(TextureSlot slot) { return static_cast<int8_t>(slot); }
			inline constexpr TextureSlot GetSlotAsEnum(int8_t slot) { return static_cast<TextureSlot>(slot); }
			
			/* Gets the number of unique TextureSlot elements supported by the engine. */
			inline constexpr int8_t GetUniqueSlotCount() { return GetSlotAsInt(TextureSlot::COUNT); }

			/* Checks if the given texture slot is valid. */
			inline bool IsValidTextureSlot(int8_t shaderSlot) 
			{
				return shaderSlot > GetSlotAsInt(TextureSlot::NONE)
					&& shaderSlot < GetSlotAsInt(TextureSlot::COUNT);
			}
			
			/* Checks if the given texture slot is valid. */
			inline bool IsValidTextureSlot(TextureSlot shaderSlot)
			{
				return IsValidTextureSlot(static_cast<int8_t>(shaderSlot));
			}

			/* Retrieves a string representation of a given TextureSlot. */
			inline const char* GetTextureSlotAsString(TextureSlot slot)
			{
				switch (slot)
				{
				case TextureSlot::DIFFUSE: return "diffuse";
				case TextureSlot::SPECULAR: return "specular";
				default:
				{
					LR_CORE_ERROR("Invalid TextureSlot: {0}!", static_cast<int>(slot));
					return "invalid";
				}
				}
			}
		}

		namespace Material
		{
			// TODO: Gotta figure out a better way to integrate props into hash calculation!
			inline size_t CalculateHash(size_t shaderHash, const std::vector<Ref<Lyra::Texture>>& textures, float shininess)
			{
				size_t materialHash = 0;
				for (const Ref<Lyra::Texture>& texture : textures)
				{
					Utils::Hash::HashCombine(materialHash, texture->GetHash());
				}
				Utils::Hash::HashCombine(materialHash, shaderHash, shininess);
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