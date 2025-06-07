#include "lrpch.h"
#include "Core/Application.h"
#include "Core/Log.h"

#if defined(LR_PLATFORM_WINDOWS) | defined(LR_PLATFORM_LINUX)

int main(int argc, char** argv)
{
	Lyra::Log::Init();

	Lyra::Application* app = Lyra::CreateApplication();
	app->Run();

	delete app;
}

#endif