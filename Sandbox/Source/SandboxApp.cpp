#include "SandboxLayer.h"

class SandboxApp : public Lyra::Application
{
public:
	SandboxApp(const Lyra::ApplicationProps& props)
		: Lyra::Application(props)
	{
		PushLayer(new SandboxLayer());
	}
	~SandboxApp() {}
};

Lyra::Application* Lyra::CreateApplication()
{
	Lyra::ApplicationProps props;
	props.windowProps = WindowProps("Standalone Game");
	props.rendererSpec.useFrameBuffer = false;
	return new SandboxApp(props);
}