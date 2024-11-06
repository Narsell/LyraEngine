#pragma once

#include "Application.h"

#ifdef LR_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Lyra::Application* app = Lyra::CreateApplication();
	app->Run();
	delete app;
}

#endif