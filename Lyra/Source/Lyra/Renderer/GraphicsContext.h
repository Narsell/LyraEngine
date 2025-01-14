#pragma once

#include "Lyra/Core.h"

namespace Lyra
{
	class LYRA_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}