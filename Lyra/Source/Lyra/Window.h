#pragma once
//TODO: Need to include this pch here or project won't compile, figure out a better solution later
#include "lrpch.h"

#include "Lyra/Core.h"
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

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps & = WindowProps());

	};

}