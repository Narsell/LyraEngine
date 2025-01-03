#include "lrpch.h"

#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Platform/GLFW/imgui_impl_glfw.h"

#include "ImGuiLayer.h"
#include "Lyra/Application.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Lyra
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		LR_CORE_INFO("ImGuiLayer::OnAttach");

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO io = ImGui::GetIO();
		Application& app = Application::GetApplication();

		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight()); // Set display size
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		void* nativeWindow = app.GetWindow().GetNativeWindow();

#ifdef LR_PLATFORM_WINDOWS:

		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(nativeWindow);
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

#endif

		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::OnDetach()
	{
		LR_CORE_INFO("ImGuiLayer::OnAttach");
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow); // Show demo window! :)

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		//LR_CORE_TRACE("ImGuiLayer::OnEvent: {0}", event);
		event.SetHandled();
	}

}