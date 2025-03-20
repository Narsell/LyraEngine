#pragma once

#include "Core/Ref.h"
#include "Core/Utils.h"

/* TEXTURE TYPE ENUM AND UTILS */
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

	enum class TextureWrap : uint8_t
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class TextureFilter : uint8_t
	{
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};

	struct Texture2DProps
	{
		Texture2DProps() = delete;
		Texture2DProps(const std::string& path, TextureType type = TextureType::NONE)
			: Path(path), Type(type) {}

		std::string Path;
		TextureType Type = TextureType::NONE;

		bool UseMipmaps = true;
		
		TextureFilter MinFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		TextureFilter MagFilter = TextureFilter::LINEAR;

		TextureWrap WrapS = TextureWrap::REPEAT;
		TextureWrap WrapT = TextureWrap::REPEAT;

		glm::vec4 BorderColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	};


	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual const std::string& GetPath() const { return m_Props.Path; }
		virtual const TextureType& GetType() const { return m_Props.Type; }
		virtual const char* GetTypeAsString() const { return Utils::Texture::TextureTypeToString(m_Props.Type); }
		virtual size_t GetHash() const = 0;

		/* Keeping count of loaded textures here... TODO: Move this to an asset manager eventually! */
		static std::unordered_map<std::string, Ref<Texture2D>> s_TexturesLoaded;
		static Ref<Texture2D> Create(const Texture2DProps& textureProps);

	protected:
		Texture2D(const Texture2DProps& textureProps)
			:   m_Props(textureProps)
		{
			if (!Utils::Texture::IsValidTextureType(m_Props.Type))
			{
				LR_CORE_WARN("Texture at path '{0}' was not given a valid type. Won't bind this texture.");
			}
		}

	protected:
		Texture2DProps m_Props;
	};
}
