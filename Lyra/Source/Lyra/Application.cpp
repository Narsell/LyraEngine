#include "lrpch.h"

#include <ranges>

#include "Application.h"
#include "Events/Event.h"

// TODO: Remove this temporary include, the renderer should take care of rendering but for now this works.
#include <glad/glad.h>

namespace Lyra
{

#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(std::unique_ptr<Window>(Window::Create()))
	{
		LR_CORE_ASSERT(!s_Instance, "There's already an application instance!");
		s_Instance = this;

		m_Window->SetEventCallback(BIND_EVENT_FN(&Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		// Dispatch window close event
		EventDispatcher dispatcher(e);
		if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(&Application::OnWindowClose)))
		{
			LR_CORE_TRACE("Dispatched WindowCloseEvent to Application::OnWindowClose");
		}

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
}