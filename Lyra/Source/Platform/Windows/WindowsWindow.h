#pragma once

#include <cstdint>
#include <string>

#include "Lyra/Core.h"
#include "Lyra/Window.h"

struct GLFWwindow;

namespace Lyra
{
	class LYRA_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		virtual void OnUpdate() override;

		virtual inline uint16_t GetWidth() const override { return m_Data.Width; }
		virtual inline uint16_t GetHeight() const override { return m_Data.Height; }
		virtual inline void* GetNativeWindow() const override { return m_Window; }

		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint16_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
