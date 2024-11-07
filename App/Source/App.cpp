#include "Lyra.h"
#include "spdlog/spdlog.h"

class SandboxApp : public Lyra::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}
};

Lyra::Application* Lyra::CreateApplication()
{
	return new SandboxApp();
}