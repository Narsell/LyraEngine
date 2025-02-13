#pragma once

#include "Lyra/Core/Core.h"
#include "Lyra/Core/Window.h"
#include "Lyra/Events/ApplicationEvent.h"
#include "Lyra/Layer/LayerStack.h"
#include "Lyra/Core/Timestep.h"
#include "Lyra/Core/Ref.h"

namespace Lyra
{
	class ImGuiLayer;

	class LYRA_API Application
	{

	public:
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& GetApplication() { return *s_Instance; }
		inline Window& GetWindow() const { return *m_Window; }

	protected:
		Application();

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;

	};

	//To be defined by client.
	extern Application* CreateApplication();

}

