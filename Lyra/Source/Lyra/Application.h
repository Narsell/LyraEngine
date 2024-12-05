#pragma once

#include <memory>

#include "Lyra/Core.h"
#include "Lyra/Window.h"
#include "Events/ApplicationEvent.h"

namespace Lyra
{

	class LYRA_API Application
	{

	public:

		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};

	//To be defined by client.
	extern Application* CreateApplication();

}

