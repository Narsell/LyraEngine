#include "Lyra.h"

class UILayer : public Lyra::Layer
{
public:
	UILayer()
		:Layer("UILayer")
	{
	}

	void OnAttach() override
	{
		LR_INFO("UILayer::OnAttach");
	};

	void OnDetach() override
	{
		LR_INFO("UILayer::OnDetach");
	};

	void OnUpdate() override
	{
		LR_TRACE("UILayer::OnUpdate");
	};

	void OnEvent(Lyra::Event& event) override
	{
		LR_TRACE("UILayer::OnEvent: {0}", event);
		event.SetHandled();
	};
};

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
	}

	void OnAttach() override
	{
		LR_INFO("GameLayer::OnAttach");
	};

	void OnDetach() override
	{
		LR_INFO("GameLayer::OnDetach");
	};

	void OnUpdate() override
	{
		//LR_TRACE("GameLayer::OnUpdate");
	};

	void OnEvent(Lyra::Event& event) override
	{
		LR_TRACE("GameLayer::OnEvent: {0}", event);
	};
};

class SandboxApp : public Lyra::Application
{
public:
	SandboxApp() 
	{
		//TODO: Fix issue where iterator is invalid if we push overlay first.
		PushLayer(new GameLayer());
		PushOverlay(new Lyra::ImGuiLayer());
	}
	~SandboxApp() {}
};

Lyra::Application* Lyra::CreateApplication()
{
	return new SandboxApp();
}