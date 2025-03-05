#pragma once

#include <cstdint>
#include <string>

#include "Lyra/Core/Core.h"
#include "Lyra/Core/Window.h"

struct GLFWwindow;

namespace Lyra
{
	class GraphicsContext;

	class LYRA_API WindowsWindow final : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		virtual void OnUpdate() override;

		virtual inline uint16_t GetWidth() const override { return m_Data.Width; }
		virtual inline uint16_t GetHeight() const override { return m_Data.Height; }
		virtual inline float GetAspectRatio() const override { return (float) m_Data.Width / (float) m_Data.Height; }

		virtual bool IsVSync() const override { return m_Data.VSync; };
		virtual void* GetNativeWindow() const override { return m_Window; }
		virtual int GetMouseInputMode() const;
		virtual void SetMouseInputMode(int InputMode) override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		virtual void SetVSync(bool enabled) override;

	private:
		void Init(const WindowProps& props);
		void Shutdown();

	private:

		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

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
