#pragma once

#include <memory>

#include "Lyra/Core.h"
#include "Lyra/Window.h"
#include "Lyra/Events/ApplicationEvent.h"
#include "Lyra/LayerStack.h"

namespace Lyra
{

	class LYRA_API Application
	{

	public:

		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

	};

	//To be defined by client.
	extern Application* CreateApplication();

}

