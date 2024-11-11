#include "Application.h"

#include "Events/MouseEvent.h"
#include "Log.h"


namespace Lyra
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Lyra::MouseButtonPressedEvent ButtonPressedEvent(16);

		if (ButtonPressedEvent.IsInCategory(Lyra::EventCategoryMouse))
		{
			LR_TRACE(ButtonPressedEvent.ToString());
		}

		if (ButtonPressedEvent.IsInCategory(Lyra::EventCategoryMouseButton))
		{
			LR_TRACE(ButtonPressedEvent.ToString());
		}

		while (true)
		{
		}
	}
}