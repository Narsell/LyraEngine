#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:	Layer("GameLayer"),
			m_Camera(0.0f, 1280.0f, 0.0f, 720.0f),
			m_CameraTranslation(m_Camera.GetPosition()),
			m_SquareTranslation(glm::vec3(640.0f, 360.0f, 0.0f)),
			m_TriangleTranslation(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_SquareTransform(glm::translate(glm::mat4(1.0f), m_SquareTranslation)),
			m_TriangleTransform(glm::translate(glm::mat4(1.0f), m_TriangleTranslation))
	{
		/* TRIANGLE SECTION */

// Create VAO and bind it.
		m_TriangleVertexArray = std::shared_ptr<Lyra::VertexArray>((Lyra::VertexArray::Create()));

		// 3 vertices in 3D space
		// Posistion (3 Comps xyz), Color (4 Comps: rgba)
		float triangleVertices[3 * 7] =
		{
			 0.0f,   0.0f,   0.0f, 0.8f, 0.3f, 0.2f, 1.0f,
			 250.0f, 0.0f,   0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
			 250.0f, 250.0f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f,
		};

		// Declaring vertex buffer layout
		Lyra::VertexLayout triangleVertexLayout
		{
			{ "a_Position", Lyra::ShaderData::Float3 },
			{ "a_Color",	Lyra::ShaderData::Float4 },
		};

		// Create vertex buffer and upload data (vertices) to GPU
		std::shared_ptr<Lyra::VertexBuffer> m_TriangleVertexBuffer((Lyra::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices), triangleVertexLayout)));
		m_TriangleVertexArray->AddVertexBuffer(m_TriangleVertexBuffer);
		triangleVertexLayout.DebugPrint("Triangle");

		// 3 indices that make up a triangle, basically the order in wich the vertices are going to be rendered.
		uint32_t triangleIndices[3] =
		{
			0, 1, 2
		};

		std::shared_ptr<Lyra::IndexBuffer> triangleIndexBuffer(Lyra::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
		m_TriangleVertexArray->AddIndexBuffer(triangleIndexBuffer);

		//Shader source code stored in plain strings for now
		std::string triangleVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_VP;
			uniform mat4 u_Model;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
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
		m_TriangleShader = std::shared_ptr<Lyra::Shader>(Lyra::Shader::Create(triangleVertexSrc, triangleFragmentSrc));

		/* SQUARE SECTION */

		m_SquareVertexArray = std::shared_ptr<Lyra::VertexArray>(Lyra::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			 0.0f,    0.0f,    0.0f,
			 150.0f,  0.0f,    0.0f,
			 150.0f,  150.0f,  0.0f,
			 0.0f,    150.0f,  0.0f
		};

		Lyra::VertexLayout squareVertexLayout
		{
			{"a_Position", Lyra::ShaderData::Float3}
		};
		std::shared_ptr<Lyra::VertexBuffer> squareVertexBuffer(Lyra::VertexBuffer::Create(squareVertices, sizeof(squareVertices), squareVertexLayout));
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
		squareVertexLayout.DebugPrint("Square");

		uint32_t squareIndices[6] =
		{
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Lyra::IndexBuffer> squareIndexBuffer(Lyra::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SquareVertexArray->AddIndexBuffer(squareIndexBuffer);

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			out vec3 v_Position;

			uniform mat4 u_VP;
			uniform mat4 u_Model;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
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

		m_SquareShader = std::shared_ptr<Lyra::Shader>(Lyra::Shader::Create(squareVertexSrc, squareFragmentSrc));

	}

	void OnAttach() override
	{
		//LR_TRACE("GameLayer::OnAttach");
		
	}

	void OnDetach() override
	{
		//LR_TRACE("GameLayer::OnDetach");
	}

	void OnUpdate(Lyra::Timestep ts) override
	{
		LR_TRACE("DeltaTime: {0} seconds ({1} miliseconds)", ts.GetSeconds(), ts.GetMiliSeconds());
		/* Clearing buffers */
		Lyra::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Lyra::RenderCommand::Clear();

		/* Actual rendering happens here */
		Lyra::Renderer::BeginScene(m_Camera);

		m_SquareShader->Bind();
		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position(x * 30.5f, y * 30.5f, 0.0f);
				m_SquareTransform = glm::translate(glm::mat4(1.0f), position) * scale;
				Lyra::Renderer::Submit(m_SquareShader, m_SquareVertexArray, m_SquareTransform);
			}
		}

		m_TriangleShader->Bind();
		Lyra::Renderer::Submit(m_TriangleShader, m_TriangleVertexArray, m_TriangleTransform);

		Lyra::Renderer::EndScene();

		/* Moving camera with keyboard input (input polling) */
		glm::vec3 currentPosition = m_Camera.GetPosition();
		glm::vec3 offsetPosition = glm::vec3(0.0f);

		if (Lyra::Input::IsKeyPressed(LR_KEY_W))
		{
			offsetPosition.y += 1.0f;
		}
		else if (Lyra::Input::IsKeyPressed(LR_KEY_S))
		{
			offsetPosition.y -= 1.0f;
		}
		if (Lyra::Input::IsKeyPressed(LR_KEY_D))
		{
			offsetPosition.x += 1.0f;
		}
		else if (Lyra::Input::IsKeyPressed(LR_KEY_A))
		{
			offsetPosition.x -= 1.0f;
		}
		if (Lyra::Input::IsKeyPressed(LR_KEY_E))
		{
			m_Camera.SetRotation(m_Camera.GetRotation() - (m_CameraRotationSpeed * ts));
		}
		else if (Lyra::Input::IsKeyPressed(LR_KEY_Q))
		{
			m_Camera.SetRotation(m_Camera.GetRotation() + (m_CameraRotationSpeed * ts));
		}

		if (offsetPosition != glm::vec3(0.0f))
		{
			offsetPosition = glm::normalize(offsetPosition) * (m_CameraSpeed * ts);
			m_CameraTranslation = currentPosition + offsetPosition;
		}

		m_Camera.SetPosition(m_CameraTranslation);

		/* Update models' transforms */

		m_TriangleTransform[3][0] = m_TriangleTranslation.x;
		m_TriangleTransform[3][1] = m_TriangleTranslation.y;
		m_TriangleTransform[3][2] = m_TriangleTranslation.z;

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("World Outline");
		ImGui::DragFloat3("Camera", &m_CameraTranslation.x);
		//ImGui::DragFloat3("Square", &m_SquareTranslation.x);
		ImGui::DragFloat3("Triangle", &m_TriangleTranslation.x);
		ImGui::End();
	}

	void OnEvent(Lyra::Event& event) override
	{
		//LR_TRACE("GameLayer::OnEvent: {0}", event);
	}

private:

	std::shared_ptr<Lyra::VertexArray> m_TriangleVertexArray;
	std::shared_ptr<Lyra::VertexArray> m_SquareVertexArray;
	std::shared_ptr<Lyra::Shader> m_TriangleShader;
	std::shared_ptr<Lyra::Shader> m_SquareShader;

	Lyra::OrthographicCamera m_Camera;

	glm::vec3 m_CameraTranslation;
	glm::vec3 m_SquareTranslation;
	glm::vec3 m_TriangleTranslation;

	glm::mat4 m_SquareTransform;
	glm::mat4 m_TriangleTransform;

	float m_CameraSpeed = 500.0f;
	float m_CameraRotationSpeed = 50.0f;
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