#pragma once

#include "Core.h"
#include "Core/Ref.h"

namespace Lyra
{
	class Shader;
	class Texture2D;
	class MaterialLibrary;

	struct MaterialProps
	{
		float shininess = 32.0f;
	};

	class LYRA_API Material
	{

	friend class MaterialLibrary;

	public:
		Material();
		Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>>& textures, const MaterialProps& matProps);
		Material(const std::vector<Ref<Texture2D>> textures);

		inline const std::vector<Ref<Texture2D>>& GetTextures() const { return m_Textures; }
		inline const Ref<Shader>& GetShader() const { return m_Shader; }
		inline size_t GetHash() const { return m_Hash; }
		inline bool IsCurrentlyBound() const { return GetHash() == s_LastBoundMatHash; }

		void UploadUniforms() const;
		void BindTextures() const;

	private:

		void SetCalculatedHash();

	private:
		MaterialProps m_Props;
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture2D>> m_Textures;

		size_t m_Hash = 0;
		static size_t s_LastBoundMatHash;
	};
}