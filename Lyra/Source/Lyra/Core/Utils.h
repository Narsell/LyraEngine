#pragma once

#include "Core.h"
#include "Core/Log.h"
#include "Scene/Texture.h"

namespace Lyra
{
	namespace Utils::Hash
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

}