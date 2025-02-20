#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:	Layer("GameLayer"),
			m_CameraController(45.0f, 16.f/9.f, 0.1f, 100.f),
			m_GridColor(glm::vec4(0.2f, 0.65f, 0.35f, 1.0f)),
			m_LightSourceColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
	{   
		
		/* TEXTURED CUBE SECTION */
		{
			m_CubeVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());

			float cubeVertices [] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Back face
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Front face
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Left face
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Right face
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom face
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Upper face
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};

			Lyra::VertexLayout cubeVertexLayout
			{
				{"a_Position", Lyra::ShaderData::Float3},
				{"a_TexCoord", Lyra::ShaderData::Float2}
			};
			cubeVertexLayout.DebugPrint("Cube");

			Ref<Lyra::VertexBuffer> cubeVertexBuffer(Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout));
			m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}

		/* SQUARE SECTION */
		{
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
		}

		float cubeVertices[] = {
			-0.5f, -0.5f, -0.5f, // Back face
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f, // Front face
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f, // Left face
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f, // Right face
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f, // Bottom face
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f, // Upper face
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f
		};

		/* LIGHT REFLECTIVE CUBE SECTION */
		{
			m_ReflectiveCubeVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());


			Lyra::VertexLayout cubeVertexLayout
			{
				{"a_Position", Lyra::ShaderData::Float3}
			};

			Ref<Lyra::VertexBuffer> cubeVertexBuffer(Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout));
			m_ReflectiveCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}
		
		/* LIGHT SOURCE CUBE SECTION */
		{
			m_LightSourceCubeVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());

			Lyra::VertexLayout cubeVertexLayout
			{
				{"a_Position", Lyra::ShaderData::Float3}
			};

			Ref<Lyra::VertexBuffer> cubeVertexBuffer(Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout));
			m_LightSourceCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}

		// Shader creation
		m_TextureShader = Lyra::Shader::Create("Assets/Shaders/Texture.glsl");
		m_ReflectiveObjShader = Lyra::Shader::Create("Assets/Shaders/ReflectionColor.glsl");
		m_LightSourceShader = Lyra::Shader::Create("Assets/Shaders/LightSource.glsl");

		// Creating and setting textures
		m_Texture = Lyra::Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_TransparentTexture = Lyra::Texture2D::Create("Assets/Textures/TransparentGreen.png");
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

		// Set the scene variables (only camera at this point)
		Lyra::Renderer::BeginScene(m_CameraController.GetCamera());

		/* Actual rendering happens here */

		/* Render square grid (Indexed) */
		m_SquareShader->Bind();
		m_SquareShader->UploadUniform_4f("u_Color", m_GridColor);
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		static glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position(x * 0.12f, y * 0.12f, 0.0f);
				Lyra::Renderer::Submit(m_SquareShader, m_SquareVertexArray, glm::translate(glm::mat4(1.0f), position) * rotation * scale);
			}
		}
		
		/* Render cube (Non-indexed) */
		m_TextureShader->Bind();
		m_TextureShader->UploadUniform_1i("u_Texture", 0);
		Lyra::Renderer::Submit(
			m_TextureShader,
			m_CubeVertexArray, 
			glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)), 
			false
		);

		/* Render light source (Non-indexed) */
		Lyra::Renderer::Submit(
			m_LightSourceShader,
			m_LightSourceCubeVertexArray,
			glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 2.0f, -0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.25f)),
			false
		);

		/* Render reflective cube (Non-indexed) */
		m_ReflectiveObjShader->Bind();
		m_ReflectiveObjShader->UploadUniform_4f("u_ObjectColor", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
		m_ReflectiveObjShader->UploadUniform_4f("u_LightColor", m_LightSourceColor);
		Lyra::Renderer::Submit(
			m_ReflectiveObjShader,
			m_ReflectiveCubeVertexArray,
			glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.5f, -0.5f)),
			false
		);

		/* Render quad (Indexed) */
		m_TextureShader->Bind();
		m_TextureShader->UploadUniform_1i("u_Texture", 1);
		Lyra::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, -0.45f, 1.0f)));

		Lyra::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("World Outline");
		//ImGui::DragFloat3("Square", &m_SquareTranslation.x);
		ImGui::ColorEdit4("Grid Color", &m_GridColor.x);
		ImGui::ColorEdit4("Light Source Color", &m_LightSourceColor.x);
		ImGui::End();
	}

	void OnEvent(Lyra::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Ref<Lyra::VertexArray> m_SquareVertexArray;
	Ref<Lyra::VertexArray> m_CubeVertexArray;
	Ref<Lyra::VertexArray> m_ReflectiveCubeVertexArray;
	Ref<Lyra::VertexArray> m_LightSourceCubeVertexArray;
	Ref<Lyra::Shader> m_SquareShader;
	Ref<Lyra::Shader> m_TextureShader;
	Ref<Lyra::Shader> m_ReflectiveObjShader;
	Ref<Lyra::Shader> m_LightSourceShader;

	Ref<Lyra::Texture2D> m_Texture, m_TransparentTexture;

	Lyra::PerspectiveCameraController m_CameraController;

	glm::vec4 m_GridColor;
	glm::vec4 m_LightSourceColor;

	glm::vec4 m_LightSourcePosition;
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