#pragma once

#include "Application.h"

#ifdef LR_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Lyra::Log::Init();

	LR_CORE_WARN("Initialized log!");
	int var = 555;
	LR_FATAL("Hello log! Here's a var: {0}", var);

	Lyra::Application* app = Lyra::CreateApplication();
	app->Run();
	delete app;
}

#endif