#include "lrpch.h"

#include "Application.h"
#include "Lyra/Core/Core.h"
#include "Lyra/Events/Event.h"
#include "Lyra/Input/Input.h"
#include "Lyra/ImGui/ImGuiLayer.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Buffer.h"
#include "Lyra/Renderer/Renderer.h"
#include "Lyra/Renderer/RenderCommand.h"

/* TODO: Temporary again! */
#include "glfw/glfw3.h"

namespace Lyra
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Window(Scope<Window>(Window::Create())),
		 m_ImGuiLayer(new ImGuiLayer())
	{
		LR_CORE_ASSERT(!s_Instance, "There's already an application instance!");
		s_Instance = this;

		Renderer::Init();

		PushOverlay(m_ImGuiLayer);
		m_Window->SetEventCallback(LR_BIND_EVENT_FN(&Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			/* Temporary glfw call, this should be platform independent */
			float time = static_cast<float>(glfwGetTime());
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		// Dispatch window events
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LR_BIND_EVENT_FN(&Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(LR_BIND_EVENT_FN(&Application::OnWindowResize));

		// Traverse to the layers bakwards to propagate events (Top layers get events first)
		// C++20 reverse ranged-based for loop HELL YEAH
		for (Layer* Layer : m_LayerStack | std::views::reverse)
		{
			Layer->OnEvent(e);
			if (e.IsHandled())
			{
				break;
			}
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}