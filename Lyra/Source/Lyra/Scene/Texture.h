#pragma once

#include <glm/glm.hpp>

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
		virtual const std::filesystem::path& GetPath() const = 0;
		virtual size_t GetHash() const = 0;

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
		Texture2DProps() = default;
		Texture2DProps(TextureType type) : Type(type) {}

		TextureType Type = TextureType::NONE;

		bool UseMipmaps = true;
		bool FlipVertically = false;
		
		TextureFilter MinFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		TextureFilter MagFilter = TextureFilter::LINEAR;

		TextureWrap WrapS = TextureWrap::REPEAT;
		TextureWrap WrapT = TextureWrap::REPEAT;

		glm::vec4 BorderColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	};


	class Texture2D : public Texture
	{

	friend class TextureLibrary;

	public:
		virtual ~Texture2D() = default;

		virtual const std::filesystem::path& GetPath() const { return m_Path; }
		virtual const TextureType& GetType() const { return m_Props.Type; }
		virtual const char* GetTypeAsString() const { return Utils::Texture::TextureTypeToString(m_Props.Type); }

	protected:
		Texture2D(const std::filesystem::path& texturePath, const Texture2DProps& textureProps);

	private:
		static Ref<Texture2D> Create(const std::filesystem::path& texturePath, const Texture2DProps& textureProps);

	protected:
		Texture2DProps m_Props;
		std::filesystem::path m_Path;

	};
}
