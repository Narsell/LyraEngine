#pragma once

#include <cstdint>
#include <string>

#include "Lyra/Core.h"
#include "Lyra/Window.h"

#include <GLFW/glfw3.h>

namespace Lyra
{
	class LYRA_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void OnUpdate() override;

		inline uint16_t GetWidth() const override { return m_Data.Width; }
		inline uint16_t GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

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
