#include "Lyra.h"

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