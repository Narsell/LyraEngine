#pragma once

#include "Core.h"

namespace Lyra
{

	class LYRA_API Application
	{

	public:

		Application();
		virtual ~Application();

		void Run();

	};

	//To be defined by client.
	Application* CreateApplication();

}

