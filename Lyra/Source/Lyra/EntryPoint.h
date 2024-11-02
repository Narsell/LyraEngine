#pragma once

#ifdef LR_PLATFORM_WINDOWS

extern Lyra::Application* Lyra::CreateApplication();

int main(int argc, char** argv)
{
	Lyra::Application* app = Lyra::CreateApplication();
	app->Run();
	delete app;
}

#endif