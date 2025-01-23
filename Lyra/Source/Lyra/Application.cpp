#include "lrpch.h"

#include "Application.h"
#include "Lyra/Core.h"
#include "Events/Event.h"
#include "Lyra/Input.h"
#include "Lyra/ImGui/ImGuiLayer.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Buffer.h"

// TODO: Remove this temporary include, the renderer should take care of rendering but for now this works.
#include <glad/glad.h>

namespace Lyra
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(std::unique_ptr<Window>(Window::Create())),
		  m_ImGuiLayer(new ImGuiLayer())
	{
		LR_CORE_ASSERT(!s_Instance, "There's already an application instance!");
		s_Instance = this;

		PushOverlay(m_ImGuiLayer);
		m_Window->SetEventCallback(LR_BIND_EVENT_FN(&Application::OnEvent));

		// Create VAO
		m_VertexArray = std::unique_ptr<VertexArray>(VertexArray::Create());

		// 3 vertices in 3D space
		// Pos x, Pos y, Pos z
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		// Create vertex buffer and upload data
		m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));

		// Set vertex array buffer layout
		VertexLayout vertexLayout;
		vertexLayout.AddElement(VertexType_Float, 3);
		m_VertexArray->SetLayout(vertexLayout);

		// 3 indices that make up a triangle, basically order in wich the vertices are going to be rendered.
		uint32_t indices[3] =
		{
			0, 1, 2
		};

		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		//Shader source code stored in plain strings for now
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			};
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 o_Color;
			in vec3 v_Position;

			void main()
			{
				o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
			};
		)";

		//Creating shader instance stored in a unique ptr
		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
		//Bind it (optional since we're binding on every frame)
		m_Shader->Bind();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}
	}

	void Application::OnEvent(Event& e)
	{
		// Dispatch window close event
		EventDispatcher dispatcher(e);
		if (dispatcher.Dispatch<WindowCloseEvent>(LR_BIND_EVENT_FN(&Application::OnWindowClose)))
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