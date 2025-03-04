#pragma once

#include "Lyra/Core/Core.h"

namespace Lyra
{

	class LYRA_API Input
	{
	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(uint8_t button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;
	};


}
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