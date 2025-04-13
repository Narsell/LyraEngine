#pragma once

#include "Core.h"
#include "Core/Timestep.h"
#include "Core/Ref.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "Layer/LayerStack.h"
#include "Renderer/RendererAPI.h"

namespace Lyra
{
	class ImGuiLayer;

	struct ApplicationProps
	{
		WindowProps windowProps;
		RendererSpecification rendererSpec;
	};

	class LYRA_API Application
	{

	public:
		virtual ~Application();

		void Run();
		void Quit();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() const { return *m_Window; }

	protected:
		Application(const ApplicationProps& props = {});

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;

	};

	//To be defined by client.
	extern Application* CreateApplication();

}

