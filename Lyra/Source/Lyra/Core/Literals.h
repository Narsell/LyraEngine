#pragma once

#include <cstdint>

namespace Lyra
{
	inline std::uint16_t operator "" _u16(unsigned long long value)
	{
		return static_cast<std::uint16_t>(value);
	}

}
