#pragma once

#include "Core.h"
#include "Core/Ref.h"
#include "Renderer/Shader.h"
#include "Scene/Texture.h"

namespace Lyra
{
	class LYRA_API Material
	{
	public:
		Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures = {});

		inline const std::vector<Ref<Texture2D>>& GetTextures() const { return m_Textures; }
		inline const Ref<Shader>& GetShader() const { return m_Shader; }
		inline size_t GetHash() const { return m_Hash; }
		inline bool IsCurrentlyBound() const { return GetHash() == s_LastBoundMaterialHash; }

		void SetTexture(const Ref<Texture2D>& texture);

		void UploadUniforms() const;
		void BindTextures() const;
	
	protected:
		static size_t s_LastBoundMaterialHash;

	private:
		void UpdateMaterialUniforms() const;

	private:

		float m_Shininess;
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture2D>> m_Textures;

		size_t m_Hash = 0;
	};
}