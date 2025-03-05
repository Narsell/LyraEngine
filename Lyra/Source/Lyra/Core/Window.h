#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "Lyra/Core/Core.h"
#include "Lyra/Events/Event.h"

namespace Lyra
{
	struct WindowProps
	{
		std::string Title;
		uint16_t Width;
		uint16_t Height;

		WindowProps(const std::string&p_Title = "Lyra Engine",
					uint16_t p_Width = 1280,
					uint16_t p_Height = 720)
			: Title(p_Title), Width(p_Width), Height(p_Height)
		{}
	};

	class LYRA_API Window
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint16_t GetWidth() const = 0;
		virtual uint16_t GetHeight() const = 0;
		virtual float GetAspectRatio() const = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual int GetMouseInputMode() const = 0;
		virtual void SetMouseInputMode(int InputMode) = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps & = WindowProps());

	};

}