#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:	Layer("GameLayer"),
			m_CameraController(45.0f, 16.f/9.f, 0.1f, 100.f),
			m_SelectedColor(glm::vec4(1.0f)),
			m_SquareTransform(glm::mat4(1.0f))
	{
		/* SQUARE SECTION */

		m_SquareVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());

		float squareVertices[5 * 4] =
		{
			 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
			 0.0f,  0.5f,  0.0f,  0.0f,  1.0f
		};

		Lyra::VertexLayout squareVertexLayout
		{
			{"a_Position", Lyra::ShaderData::Float3},
			{"a_TexCoord", Lyra::ShaderData::Float2}
		};
		Ref<Lyra::VertexBuffer> squareVertexBuffer(Lyra::VertexBuffer::Create(squareVertices, sizeof(squareVertices), squareVertexLayout));
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
		squareVertexLayout.DebugPrint("Square");

		uint32_t squareIndices[6] =
		{
			0, 1, 2, 2, 3, 0
		};

		Ref<Lyra::IndexBuffer> squareIndexBuffer(Lyra::IndexBuffer::Create(squareIndices, sizeof(squareIndices)));
		m_SquareVertexArray->AddIndexBuffer(squareIndexBuffer);

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			out vec3 v_Position;

			uniform mat4 u_VP;
			uniform mat4 u_Model;
			uniform vec4 u_Color;
			
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

			uniform vec4 u_Color;

			void main()
			{
				o_Color = u_Color;
			};
		)";

		m_SquareShader = Lyra::Shader::Create("Square Shader", squareVertexSrc, squareFragmentSrc);

		m_TextureShader = Lyra::Shader::Create("Assets/Shaders/Texture.glsl");

		m_Texture = Lyra::Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_TransparentTexture = Lyra::Texture2D::Create("Assets/Textures/ChernoLogo.png");
		m_Texture->Bind(0);
		m_TransparentTexture->Bind(1);
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
		m_CameraController.OnUpdate(ts);
		/* Clearing buffers */
		Lyra::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Lyra::RenderCommand::Clear();

		/* Actual rendering happens here */
		Lyra::Renderer::BeginScene(m_CameraController.GetCamera());

		m_SquareShader->Bind();
		m_SquareShader->UploadUniform_4f("u_Color", m_SelectedColor);
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		static glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position(x * 0.12f, y * 0.12f, 0.0f);
				m_SquareTransform = glm::translate(glm::mat4(1.0f), position) * rotation * scale;
				Lyra::Renderer::Submit(m_SquareShader, m_SquareVertexArray, m_SquareTransform);
			}
		}
		
		m_TextureShader->Bind();

		m_TextureShader->UploadUniform_1i("u_Texture", 0);
		Lyra::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.3f, 0.0f)));

		m_TextureShader->UploadUniform_1i("u_Texture", 1);
		Lyra::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.45f, -0.45f, 0.0f)));

		//m_TriangleShader->Bind();
		//Lyra::Renderer::Submit(m_TriangleShader, m_TriangleVertexArray, m_TriangleTransform);

		Lyra::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("World Outline");
		//ImGui::DragFloat3("Square", &m_SquareTranslation.x);
		ImGui::ColorEdit4("Square Color", &m_SelectedColor.x);
		ImGui::End();
	}

	void OnEvent(Lyra::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Ref<Lyra::VertexArray> m_SquareVertexArray;
	Ref<Lyra::Shader> m_SquareShader;
	Ref<Lyra::Shader> m_TextureShader;

	Ref<Lyra::Texture2D> m_Texture, m_TransparentTexture;

	Lyra::PerspectiveCameraController m_CameraController;

	glm::vec4 m_SelectedColor;

	glm::mat4 m_SquareTransform;
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