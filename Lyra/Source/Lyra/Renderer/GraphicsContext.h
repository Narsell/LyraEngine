#pragma once

#include "Lyra/Core/Core.h"

namespace Lyra
{
	/* Interface to handle each graphics API context initialization */
	class LYRA_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}