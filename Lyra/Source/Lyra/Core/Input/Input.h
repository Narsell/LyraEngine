#pragma once

#include "Core.h"

/* STOLEN FROM GLFW AGAIN */
#define LR_CURSOR                  0x00033001
#define LR_STICKY_KEYS             0x00033002
#define LR_STICKY_MOUSE_BUTTONS    0x00033003
#define LR_LOCK_KEY_MODS           0x00033004
#define LR_RAW_MOUSE_MOTION        0x00033005
#define LR_UNLIMITED_MOUSE_BUTTONS 0x00033006

#define LR_CURSOR_NORMAL          0x00034001
#define LR_CURSOR_HIDDEN          0x00034002
#define LR_CURSOR_DISABLED        0x00034003
#define LR_CURSOR_CAPTURED        0x00034004

namespace Lyra
{
	class LYRA_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(uint8_t button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
