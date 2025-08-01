#pragma once

#include <vector>
#include <filesystem>

#include "Core/Log.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"
#include "Scene/Model.h"

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
			inline size_t CalculateHash(const std::filesystem::path& path, const TextureProps& props)
			{
				size_t textureHash = 0;
				Hash::HashCombine(textureHash, path.string(), props);
				return textureHash;
			}

			inline size_t CalculateListHash(const std::vector<std::filesystem::path>& paths, const TextureProps& props)
			{
				size_t combinedHash = 0;
				for (const auto& path : paths)
				{
					Hash::HashCombine(combinedHash, path.string(), props);
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
			inline size_t CalculateHash(size_t shaderHash, const std::vector<Ref<Lyra::Texture>>& textures, const MaterialProps& props) 
			{
				size_t materialHash = 0;
				for (const Ref<Lyra::Texture>& texture : textures)
				{
					Utils::Hash::HashCombine(materialHash, texture->GetHash());
				}
				Utils::Hash::HashCombine(materialHash, shaderHash, props);
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
			inline size_t CalculateHash(const std::string& path, const Lyra::ModelProps& props) 
			{
				size_t modelHash = 0;
				Hash::HashCombine(modelHash, path, props);
				return modelHash;
			}
		}
	}
}

// Template specialization for custom types, it declares their hashing functions.
// TODO: Use something like magic_get to implement simple static reflection
// This would allow me to generate a hash on these types for all of their fields by iteraing over them. 
template <>
struct std::hash<Lyra::MaterialProps>
{
	std::size_t operator()(const Lyra::MaterialProps& data)
	{
		size_t hash;
		Lyra::Utils::Hash::HashCombine(hash, data.shininess);
		return hash;
	}
};

template <>
struct std::hash<Lyra::TextureProps>
{
	std::size_t operator()(const Lyra::TextureProps& data)
	{
		size_t hash;
		Lyra::Utils::Hash::HashCombine(hash, 
			data.FlipVertically,
			data.MagFilter,
			data.MinFilter,
			data.Slot,
			data.UseMipmaps,
			data.WrapR,
			data.WrapS,
			data.WrapT
		);
		return hash;
	}
};

template <>
struct std::hash<Lyra::ModelProps>
{
	std::size_t operator()(const Lyra::ModelProps& data)
	{
		size_t hash;
		Lyra::Utils::Hash::HashCombine(hash, data.textureFlipOverride);
		return hash;
	}
};