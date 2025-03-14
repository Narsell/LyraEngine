#pragma once

#include "Lyra/Core/Core.h"
#include "Lyra/Core/Ref.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Texture.h"
#include "Lyra/Material.h"

namespace Lyra
{
	class LYRA_API Material
	{
	public:
		Material(const Ref<Shader>& shader, const std::vector<Ref<Texture2D>> textures = {});

		inline const Ref<Shader>& GetShader() const { return m_Shader; }

		void SetTexture(const Ref<Texture2D>& texture);

		void UploadData();

		// TODO: Improve this... this is not sufficient, I need to combine multiple hashes (shader, textures, material uniform values)
		inline size_t GetHash() const { return m_Shader->GetHash(); }

	private:

		float m_Shininess;
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture2D>> m_Textures;

		size_t m_Hash;
	};
}