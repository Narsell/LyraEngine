#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:	Layer("GameLayer"),
			m_CameraController(45.0f, 16.f/9.f, 0.1f, 100.f),
			m_LightSourceColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_CubeRotation(0.0f),
			m_CubeRotationSpeed(35.0f),
			m_CubePosition(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_LightSourcePosition(glm::vec3(0.0, 0.0f, 0.0f)),
			m_SpecularStrength(0.65f),
			m_AmbientStrength(0.125f),
			m_ShininessFactor(32.f),
			m_LightSourceAngle(0.0f),
			m_LightSourceOrbitRadius(2.0f),
			m_LightSourceSpeed(1.7f)
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

		/* QUAD SECTION */
		{
			m_QuadVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());

			float quadVertices[5 * 4] =
			{
				 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
				 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
				 0.0f,  0.5f,  0.0f,  0.0f,  1.0f
			};

			Lyra::VertexLayout quadVertexLayout
			{
				{"a_Position", Lyra::ShaderData::Float3},
				{"a_TexCoord", Lyra::ShaderData::Float2}
			};
			Ref<Lyra::VertexBuffer> squareVertexBuffer(Lyra::VertexBuffer::Create(quadVertices, sizeof(quadVertices), quadVertexLayout));
			m_QuadVertexArray->AddVertexBuffer(squareVertexBuffer);
			quadVertexLayout.DebugPrint("Square");

			uint32_t quadIndices[6] =
			{
				0, 1, 2, 2, 3, 0
			};

			Ref<Lyra::IndexBuffer> quadIndexBuffer(Lyra::IndexBuffer::Create(quadIndices, sizeof(quadIndices)));
			m_QuadVertexArray->AddIndexBuffer(quadIndexBuffer);

			std::string quadVertexSrc = R"(
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

			std::string quadFragmentSrc = R"(
				#version 330 core
			
				out vec4 o_Color;
				in vec3 v_Position;

				uniform vec4 u_Color;

				void main()
				{
					o_Color = u_Color;
				};
			)";

			m_QuadShader = Lyra::Shader::Create("Quad Shader", quadVertexSrc, quadFragmentSrc);
		}

		/* LIGHT REFLECTIVE CUBE SECTION */
		{
			m_ReflectiveCubeVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());

			/* Pos (x, y, z), Normals (x, y, z), Texture Coords (x, y) */
			float cubeVertices[] = {
				-0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0, 0.0f, 0.0f,  // Back face
				 0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0, 1.0f, 0.0f, 
				 0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0, 1.0f, 1.0f, 
				 0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0, 1.0f, 1.0f, 
				-0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0, 0.0f, 1.0f, 
				-0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0, 0.0f, 0.0f, 
													  		   
				-0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,  0.0f, 0.0f, // Front face
				 0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,	 1.0f, 0.0f, 
				 0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,	 1.0f, 1.0f, 
				 0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,	 1.0f, 1.0f, 
				-0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,	 0.0f, 1.0f, 
				-0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,	 0.0f, 0.0f, 
													  		   
				-0.5f,  0.5f,  0.5f, -1.0, 0.0, 0.0, 1.0f, 0.0f,  // Left face
				-0.5f,  0.5f, -0.5f, -1.0, 0.0, 0.0, 1.0f, 1.0f, 
				-0.5f, -0.5f, -0.5f, -1.0, 0.0, 0.0, 0.0f, 1.0f, 
				-0.5f, -0.5f, -0.5f, -1.0, 0.0, 0.0, 0.0f, 1.0f, 
				-0.5f, -0.5f,  0.5f, -1.0, 0.0, 0.0, 0.0f, 0.0f, 
				-0.5f,  0.5f,  0.5f, -1.0, 0.0, 0.0, 1.0f, 0.0f, 
													  		   
				 0.5f,  0.5f,  0.5f, 1.0, 0.0, 0.0,  1.0f, 0.0f,  // Right face
				 0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0,	 1.0f, 1.0f, 
				 0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0,	 0.0f, 1.0f, 
				 0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0,	 0.0f, 1.0f, 
				 0.5f, -0.5f,  0.5f, 1.0, 0.0, 0.0,	 0.0f, 0.0f, 
				 0.5f,  0.5f,  0.5f, 1.0, 0.0, 0.0,	 1.0f, 0.0f, 
													  		   
				-0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 0.0f, 1.0f,  // Bottom face
				 0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 1.0f, 1.0f, 
				 0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 1.0f, 0.0f, 
				 0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 1.0f, 0.0f, 
				-0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 0.0f, 0.0f, 
				-0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 0.0f, 1.0f, 
													  		   
				-0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0,  0.0f, 1.0f, // Upper face
				 0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0,	 1.0f, 1.0f, 
				 0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0,	 1.0f, 0.0f, 
				 0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0,	 1.0f, 0.0f, 
				-0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0,	 0.0f, 0.0f, 
				-0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0,	 0.0f, 1.0f
			};

			Lyra::VertexLayout cubeVertexLayout
			{
				{"a_Position", Lyra::ShaderData::Float3},
				{"a_Normal", Lyra::ShaderData::Float3},
				{"a_TexCoord", Lyra::ShaderData::Float2}
			};

			Ref<Lyra::VertexBuffer> cubeVertexBuffer(Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout));
			m_ReflectiveCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}
		
		/* LIGHT SOURCE CUBE SECTION */
		{
			m_LightSourceCubeVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());

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

			Lyra::VertexLayout cubeVertexLayout
			{
				{"a_Position", Lyra::ShaderData::Float3}
			};

			Ref<Lyra::VertexBuffer> cubeVertexBuffer(Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout));
			m_LightSourceCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}

		// Shader creation
		m_TextureShader = Lyra::Shader::Create("Assets/Shaders/Texture.glsl");
		m_PhongShader = Lyra::Shader::Create("Assets/Shaders/PhongModel.glsl");
		m_LightSourceShader = Lyra::Shader::Create("Assets/Shaders/LightSource.glsl");

		// Creating and setting textures
		m_Texture = Lyra::Texture2D::Create("Assets/Textures/Cube.jpg");
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
		/* Light source orbit */
		m_LightSourceAngle += m_LightSourceSpeed * ts.GetSeconds();
		float height = 0.5f * glm::sin(m_LightSourceAngle * 2.0f);
		m_LightSourceAngle = glm::mod(m_LightSourceAngle, glm::two_pi<float>());
		m_LightSourcePosition = m_CubePosition + glm::vec3(
			m_LightSourceOrbitRadius * glm::cos(m_LightSourceAngle),
			height,
			m_LightSourceOrbitRadius * glm::sin(m_LightSourceAngle)
		);
		/* Cube rotation */
		m_CubeRotation += m_CubeRotationSpeed * ts.GetSeconds();
		m_CubeRotation = glm::mod(m_CubeRotation, 360.f);

		m_CameraController.OnUpdate(ts);

		/* Clearing buffers */
		Lyra::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Lyra::RenderCommand::Clear();

		// Set the scene variables (only camera at this point)
		Lyra::Renderer::BeginScene(m_CameraController.GetCamera());

		/* Actual rendering happens here */
		
		/* Render cube (Non-indexed) */
		m_TextureShader->Bind();
		m_TextureShader->UploadUniform_1i("u_Texture", 0);
		Lyra::Renderer::Submit(
			m_TextureShader,
			m_CubeVertexArray, 
			glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.15f, -6.0f)), 
			false
		);
		
		/* Render light source (Non-indexed) */
		m_LightSourceShader->Bind();
		m_LightSourceShader->UploadUniform_4f("u_Color", m_LightSourceColor);
		Lyra::Renderer::Submit(
			m_LightSourceShader,
			m_LightSourceCubeVertexArray,
			glm::translate(glm::mat4(1.0f), m_LightSourcePosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
			false
		);

		/* Render reflective cube (Non-indexed) */
		m_PhongShader->Bind();
		m_PhongShader->UploadUniform_1i("u_Texture", 0);
		m_PhongShader->UploadUniform_4f("u_LightColor", m_LightSourceColor);
		m_PhongShader->UploadUniform_3f("u_LightPosition", m_LightSourcePosition);
		m_PhongShader->UploadUniform_1f("u_AmbientStrenght", m_AmbientStrength);
		m_PhongShader->UploadUniform_1f("u_SpecularStrenght", m_SpecularStrength);
		m_PhongShader->UploadUniform_1f("u_ShininessFactor", m_ShininessFactor);
		Lyra::Renderer::Submit(
			m_PhongShader,
			m_ReflectiveCubeVertexArray,
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))) * glm::translate(glm::mat4(1.0f), m_CubePosition),
			false
		);

		/* Render quad (Indexed) */
		m_TextureShader->Bind();
		m_TextureShader->UploadUniform_1i("u_Texture", 1);
		Lyra::Renderer::Submit(m_TextureShader, m_QuadVertexArray, glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0, -4.5f)));

		Lyra::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("World Outline");
		ImGui::Text("Light paramters");
		ImGui::ColorEdit3("LightSource Color", &m_LightSourceColor.x);
		ImGui::DragFloat("Ambient Strength", &m_AmbientStrength, 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Specular Strength", &m_SpecularStrength, 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Shininess Factor", &m_ShininessFactor, 1.f, 2.f, 256.f);
		ImGui::Text("Light orbit");
		ImGui::DragFloat("LightSource Radius", &m_LightSourceOrbitRadius, 0.1f);
		ImGui::DragFloat("LightSource Speed", &m_LightSourceSpeed, 0.1f);
		ImGui::NewLine();
		ImGui::Text("Object transforms");
		ImGui::DragFloat3("LightSource Position", &m_LightSourcePosition.x, 0.1f);
		ImGui::DragFloat3("Cube Position", &m_CubePosition.x, 0.1f);
		ImGui::DragFloat("Cube Rotation Speed", &m_CubeRotationSpeed, 0.1f, 0.0f, 360.f);
		ImGui::End();
	}

	void OnEvent(Lyra::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Ref<Lyra::VertexArray> m_QuadVertexArray;
	Ref<Lyra::VertexArray> m_CubeVertexArray;
	Ref<Lyra::VertexArray> m_ReflectiveCubeVertexArray;
	Ref<Lyra::VertexArray> m_LightSourceCubeVertexArray;
	Ref<Lyra::Shader> m_QuadShader;
	Ref<Lyra::Shader> m_TextureShader;
	Ref<Lyra::Shader> m_PhongShader;
	Ref<Lyra::Shader> m_LightSourceShader;

	Ref<Lyra::Texture2D> m_Texture, m_TransparentTexture;

	Lyra::PerspectiveCameraController m_CameraController;

	glm::vec4 m_LightSourceColor;

	float m_CubeRotation;
	float m_CubeRotationSpeed;
	glm::vec3 m_CubePosition;
	glm::vec3 m_LightSourcePosition;

	float m_SpecularStrength;
	float m_AmbientStrength;
	float m_ShininessFactor;

	float m_LightSourceAngle;
	float m_LightSourceOrbitRadius;
	float m_LightSourceSpeed;
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