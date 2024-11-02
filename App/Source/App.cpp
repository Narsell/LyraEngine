#include "Lyra.h"

class SandboxApp : public Lyra::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}
};

Lyra::Application* CreateApplication()
{
	return new SandboxApp();
}