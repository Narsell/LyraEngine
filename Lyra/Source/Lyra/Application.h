#pragma once

#include <memory>

#include "Lyra/Core.h"
#include "Lyra/Window.h"
#include "Lyra/Events/ApplicationEvent.h"
#include "Lyra/LayerStack.h"

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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;

		static Application* s_Instance;

	};

	//To be defined by client.
	extern Application* CreateApplication();

}

