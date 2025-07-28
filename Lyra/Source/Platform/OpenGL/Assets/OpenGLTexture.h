#pragma once

#include <glad/glad.h>
#include <string>
#include <filesystem>

#include "Assets/Texture.h"
#include "Platform/OpenGL/Utils/OpenGLUtils.h"

namespace Lyra
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::filesystem::path& texturePath, const TextureProps& textureProps);
		~OpenGLTexture2D();

		virtual u_int32_t GetRendererId() const override { return m_RendererId; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const TextureProps& GetProps() const override { return m_Props; }
		virtual const std::filesystem::path& GetPath() const override { return m_Path; }
		virtual size_t GetHash() const override { return m_Hash; }
		virtual void Bind() const override;
	
	private:
		void LoadTexture();
		uint32_t GetCubemapCord(ECubemapCoordinate cubeMapCord);

	private:
		uint32_t m_RendererId = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		TextureProps m_Props;
		const std::filesystem::path m_Path;	
		size_t m_Hash = 0;

		int8_t m_CachedTexSlot = -1;	
	};

}