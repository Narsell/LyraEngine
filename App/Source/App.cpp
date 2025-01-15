#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
	}

	void OnAttach() override
	{
		//LR_TRACE("GameLayer::OnAttach");
	}

	void OnDetach() override
	{
		//LR_TRACE("GameLayer::OnDetach");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Button("Click me!");
		ImGui::End();
	}

	void OnEvent(Lyra::Event& event) override
	{
		//LR_TRACE("GameLayer::OnEvent: {0}", event);
	}
};

class SandboxApp : public Lyra::Application
{
public:
	SandboxApp()
	{
		//TODO: Fix issue where iterator is invalid if we push overlay first.
		PushLayer(new GameLayer());
	}
	~SandboxApp() {}
};

Lyra::Application* Lyra::CreateApplication()
{
	return new SandboxApp();
}