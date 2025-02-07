#include "lrpch.h"

#include "Application.h"
#include "Lyra/Core.h"
#include "Events/Event.h"
#include "Lyra/Input.h"
#include "Lyra/ImGui/ImGuiLayer.h"
#include "Lyra/Renderer/Shader.h"
#include "Lyra/Renderer/Buffer.h"
#include "Lyra/Renderer/Renderer.h"
#include "Lyra/Renderer/RenderCommand.h"
/* EXTREMELY TEMPORARY */
#include "imgui.h"
/* EXTREMELY TEMPORARY */

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

		/* TRIANGLE SECTION */

		// Create VAO and bind it.
		m_TriangleVertexArray = std::shared_ptr<VertexArray>((VertexArray::Create()));

		// 3 vertices in 3D space
		// Posistion (3 Comps xyz), Color (4 Comps: rgba)
		float triangleVertices[3 * 7] = 
		{
			 250.0f, 250.0f, 0.0f, 0.8f, 0.3f, 0.2f, 1.0f,
			 400.0f, 250.0f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
			 325.0f, 400.0f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f,
		};

		// Declaring vertex buffer layout
		VertexLayout triangleVertexLayout
		{
			{ "a_Position", ShaderData::Float3 },
			{ "a_Color",	ShaderData::Float4 },
		};

		// Create vertex buffer and upload data (vertices) to GPU
		std::shared_ptr<VertexBuffer> m_TriangleVertexBuffer((VertexBuffer::Create(triangleVertices, sizeof(triangleVertices), triangleVertexLayout)));
		m_TriangleVertexArray->AddVertexBuffer(m_TriangleVertexBuffer);
		triangleVertexLayout.DebugPrint("Triangle");

		// 3 indices that make up a triangle, basically the order in wich the vertices are going to be rendered.
		uint32_t triangleIndices[3] =
		{
			0, 1, 2
		};

		std::shared_ptr<IndexBuffer> triangleIndexBuffer(IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
		m_TriangleVertexArray->AddIndexBuffer(triangleIndexBuffer);

		//Shader source code stored in plain strings for now
		std::string triangleVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_MVP;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_MVP * vec4(a_Position, 1.0);
			};
		)";

		std::string triangleFragmentSrc = R"(
			#version 330 core
			
			out vec4 o_Color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
				o_Color = v_Color;
			};
		)";

		//Creating shader instance - Compiles and links shader source code.
		m_TriangleShader = std::make_shared<Shader>(triangleVertexSrc, triangleFragmentSrc);

		/* SQUARE SECTION */

		m_SquareVertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			 0.0f,    0.0f,    0.0f,
			 150.0f,  0.0f,    0.0f,
			 150.0f,  150.0f,  0.0f,
			 0.0f,    150.0f,  0.0f
		};

		VertexLayout squareVertexLayout
		{
			{"a_Position", ShaderData::Float3}
		};
		std::shared_ptr<VertexBuffer> squareVertexBuffer(VertexBuffer::Create(squareVertices, sizeof(squareVertices), squareVertexLayout));
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
		squareVertexLayout.DebugPrint("Square");

		uint32_t squareIndices[6] =
		{
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIndexBuffer(IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SquareVertexArray->AddIndexBuffer(squareIndexBuffer);

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			out vec3 v_Position;

			uniform mat4 u_MVP;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_MVP * vec4(a_Position, 1.0);
			};
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			out vec4 o_Color;
			in vec3 v_Position;

			void main()
			{
				o_Color = vec4(0.2, 0.8, 0.3, 1.0);
			};
		)";

		m_SquareShader = std::make_shared<Shader>(squareVertexSrc, squareFragmentSrc);

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		Camera camera;

		const std::array<std::shared_ptr<Shader>, 2> shaders =
		{
			m_SquareShader,
			m_TriangleShader
		};

		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			RenderCommand::Clear();

			Renderer::BeginScene(camera, shaders);

			m_SquareShader->Bind();
			Renderer::Submit(m_SquareVertexArray);

			m_TriangleShader->Bind();
			Renderer::Submit(m_TriangleVertexArray);

			Renderer::EndScene();

			m_ImGuiLayer->Begin();

			/* EXTREMELY TEMPORARY */
			ImGui::Begin("Camera Transform");
			ImGui::DragFloat3("Translation", &cameraPosition.x);
			ImGui::End();
			/* EXTREMELY TEMPORARY  */

			camera.SetPosition(cameraPosition);

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