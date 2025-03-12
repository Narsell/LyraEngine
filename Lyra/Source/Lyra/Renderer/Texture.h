#pragma once

#include "Lyra/Core/Ref.h"

namespace Lyra
{
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
		static Ref<Texture2D> Create(const std::string& path);
		/* Keeping count of loaded textures here... TODO: Move this to an asset manager eventually! */
		static std::unordered_map<std::string, Ref<Texture2D>> s_TexturesLoaded;
	};
}
