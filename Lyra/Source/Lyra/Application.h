#pragma once

#include "Lyra/Core.h"
#include "Lyra/Window.h"

namespace Lyra
{

	class LYRA_API Application
	{

	public:

		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};

	//To be defined by client.
	extern Application* CreateApplication();

}

