#pragma once

#include "Core/Ref.h"

namespace Lyra
{

	enum class TextureType : int8_t
	{
		NONE = -1,
		DIFFUSE,
		SPECULAR,


		COUNT = SPECULAR + 1
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::string& GetPath() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(TextureType textureType = TextureType::NONE) : m_Type(textureType) {}
		virtual ~Texture2D() = default;
		TextureType GetType() const { return m_Type; }

		/* Keeping count of loaded textures here... TODO: Move this to an asset manager eventually! */
		static std::unordered_map<std::string, Ref<Texture2D>> s_TexturesLoaded;
		static Ref<Texture2D> Create(const std::string& path, TextureType textureType = TextureType::NONE);

	private:
		TextureType m_Type = TextureType::NONE;
	};
}
