#pragma once

#include <glm/glm.hpp>

#include "Core/Ref.h"
#include "Core/Utils.h"

/* TEXTURE TYPE ENUM AND UTILS */

namespace Lyra
{
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

	struct TextureProps
	{
		TextureProps() = default;
		TextureProps(TextureSlot slot) : Slot(slot) {}

		TextureSlot Slot = TextureSlot::NONE;
		
		bool UseMipmaps = true;
		bool FlipVertically = false;
		
		TextureFilter MinFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		TextureFilter MagFilter = TextureFilter::LINEAR;

		TextureWrap WrapS = TextureWrap::REPEAT;
		TextureWrap WrapT = TextureWrap::REPEAT;
		TextureWrap WrapR = TextureWrap::REPEAT;

		glm::vec4 BorderColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetRendererId() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const TextureProps& GetProps() const = 0;
		virtual const std::filesystem::path& GetPath() const = 0;
		virtual size_t GetHash() const = 0;

		virtual void Bind() const = 0;
	};

	class Texture2D : public Texture
	{
	friend class TextureLibrary;

	public:
		virtual ~Texture2D() = default;

	protected:
		Texture2D() = default;

	private:
		static Ref<Texture2D> Create(const std::filesystem::path& texturePath, const TextureProps& textureProps);
	};

	enum class ECubemapCoordinate : int8_t
	{
		NONE = -1,
		POSITIVE_X, NEGATIVE_X,
		POSITIVE_Y, NEGATIVE_Y,
		POSITIVE_Z, NEGATIVE_Z
	};

	class CubemapTexture : public Texture
	{
	friend class TextureLibrary;

	public:
		virtual ~CubemapTexture() = default;
	
	protected:
		CubemapTexture() = default;

	private:
		static Ref<CubemapTexture> Create(const std::vector<std::filesystem::path>& texturePaths, const TextureProps& textureProps);
	};
}
