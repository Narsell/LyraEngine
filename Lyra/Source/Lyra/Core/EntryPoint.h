#pragma once

#include "Core/Application.h"
#include "Core/Log.h"

#ifdef LR_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Lyra::Log::Init();

	Lyra::Application* app = Lyra::CreateApplication();
	app->Run();

	delete app;
}

#endif