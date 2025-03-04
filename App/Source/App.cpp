#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:	Layer("GameLayer"),
			m_CameraController(45.0f, 16.f/9.f, 0.1f, 100.f),
			m_CubeRotation(0.0f),
			m_CubeRotationSpeed(12.0f),
			m_CubePosition(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_LightPosition(glm::vec3(1.5f, 1.0f, -0.0f)),
			m_ShininessFactor(32.f),
			m_LightAmbientColor(glm::vec3(0.2f, 0.2f, 0.2f)),
			m_LightDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f)),
			m_LightSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f)),
			m_LightSourceAngle(0.0f),
			m_LightSourceOrbitRadius(2.0f),
			m_LightSourceSpeed(1.7f)
	{   
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
		
		/* PHONG MODEL CUBE SECTION */
		{
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
			cubeVertexLayout.DebugPrint("Phong model cube");
			Ref<Lyra::VertexBuffer> cubeVertexBuffer(Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout));

			m_CubeVertexArray = Ref<Lyra::VertexArray>(Lyra::VertexArray::Create());
			m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
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
		m_Texture = Lyra::Texture2D::Create("Assets/Textures/Container.png");
		m_TextureSpecular = Lyra::Texture2D::Create("Assets/Textures/Container_specular.png");
		m_TransparentTexture = Lyra::Texture2D::Create("Assets/Textures/TransparentGreen.png");
		m_Texture->Bind(0);
		m_TextureSpecular->Bind(1);
		m_TransparentTexture->Bind(2);
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
		m_Time += ts;
		m_CameraController.OnUpdate(ts);

		/* Light source orbit */
		//m_LightSourceAngle += m_LightSourceSpeed * ts.GetSeconds();
		//m_LightSourceAngle = glm::mod(m_LightSourceAngle, glm::two_pi<float>());
		//m_LightPosition = m_CubePosition + glm::vec3(
		//	m_LightSourceOrbitRadius * glm::cos(m_LightSourceAngle),
		//	m_LightSourceOrbitRadius * glm::sin(m_LightSourceAngle),
		//	0.0f
		//);

		/* Cube rotation */
		m_CubeRotation += m_CubeRotationSpeed * ts.GetSeconds();
		m_CubeRotation = glm::mod(m_CubeRotation, 360.f);

		/* Clearing buffers */
		Lyra::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Lyra::RenderCommand::Clear();

		// Set the scene variables (only camera at this point)
		Lyra::Renderer::BeginScene(m_CameraController.GetCamera());

		/* Actual rendering happens here */
				
		/* Render light source (Non-indexed) */
		//m_LightSourceShader->Bind();
		//m_LightSourceShader->UploadUniform_3f("u_Color", m_LightDiffuseColor);
		//Lyra::Renderer::Submit(
		//	m_LightSourceShader,
		//	m_LightSourceCubeVertexArray,
		//	glm::translate(glm::mat4(1.0f), m_LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
		//	false
		//);

		/* Render cubes (Non-indexed) */
		m_PhongShader->Bind();
		m_PhongShader->UploadUniform_1i("u_Material.diffuse", 0);
		m_PhongShader->UploadUniform_1i("u_Material.specular", 1);
		m_PhongShader->UploadUniform_1f("u_Material.shininess", m_ShininessFactor);
		// Light position should be sent in view space since that's the coordinate space the fragment shader uses :)
		//m_PhongShader->UploadUniform_3f("u_Light.position", glm::vec3(m_CameraController.GetCamera().GetViewMatrix() * glm::vec4(m_LightPosition, 1.0)));
		m_PhongShader->UploadUniform_3f("u_Light.ambient", m_LightAmbientColor);
		m_PhongShader->UploadUniform_3f("u_Light.diffuse", m_LightDiffuseColor);
		m_PhongShader->UploadUniform_3f("u_Light.specular", m_LightSpecularColor);
		m_PhongShader->UploadUniform_1f("u_Light.constAttenuation", 1.0f);
		m_PhongShader->UploadUniform_1f("u_Light.linearAttenuation", 0.09f);
		m_PhongShader->UploadUniform_1f("u_Light.quadAttenuation", 0.032f);
		m_PhongShader->UploadUniform_3f("u_Light.direction", m_CameraController.GetCamera().GetForward());
		m_PhongShader->UploadUniform_1f("u_Light.innerCutoffCosine", glm::cos(glm::radians(12.5f)));
		m_PhongShader->UploadUniform_1f("u_Light.outerCutoffCosine", glm::cos(glm::radians(17.5f)));

		for (float i = 0; i < 10; i++)
		{
			glm::vec3 offset = glm::vec3(
				(int)i % 2 == 0 ?  i / 2.0f : -i / 2.0f,
				(int)i % 2 == 0 ? -i / 2.0f :  i / 2.0f,
				- i
			);

			Lyra::Renderer::Submit(
				m_PhongShader,
				m_CubeVertexArray,
				glm::rotate(glm::mat4(1.0f), glm::radians(i == 0 ? m_CubeRotation : 0.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))) * glm::translate(glm::mat4(1.0f), m_CubePosition + offset),
				false
			);
		}


		/* Render quad (Indexed) */
		m_TextureShader->Bind();
		m_TextureShader->UploadUniform_1i("u_Texture", 2);
		Lyra::Renderer::Submit(m_TextureShader, m_QuadVertexArray, glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0, -4.5f)));

		Lyra::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Properties");
		ImGui::Text("Light");
		ImGui::DragFloat3("Position", &m_LightPosition.x, 0.1f);
		ImGui::ColorEdit3("Ambient##Light", &m_LightAmbientColor.x);
		ImGui::ColorEdit3("Diffuse##Light", &m_LightDiffuseColor.x);
		ImGui::ColorEdit3("Specular##Light", &m_LightSpecularColor.x);
		ImGui::Text("Material");
		ImGui::SliderFloat("Shininess", &m_ShininessFactor, 2.f, 256.f);
		ImGui::Text("Light orbit");
		ImGui::SliderFloat("Radius", &m_LightSourceOrbitRadius, 1.0f, 10.0f);
		ImGui::SliderFloat("Speed", &m_LightSourceSpeed, 0.0f, 25.0f);
		ImGui::Text("Obj Properties");
		ImGui::DragFloat3("Cube Position", &m_CubePosition.x, 0.1f);
		ImGui::SliderFloat("Cube Rot Speed", &m_CubeRotationSpeed, 0.0f, 100.0f);
		ImGui::Text("Camera Position:");
		ImGui::SameLine();
		ImGui::Value("X", m_CameraController.GetCamera().GetPosition().x);
		ImGui::SameLine();
		ImGui::Value("Y", m_CameraController.GetCamera().GetPosition().y);
		ImGui::SameLine();
		ImGui::Value("Z", m_CameraController.GetCamera().GetPosition().z);
		ImGui::End();
	}

	void OnEvent(Lyra::Event& e) override
	{
		m_CameraController.OnEvent(e);

		Lyra::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Lyra::KeyPressedEvent>(LR_BIND_EVENT_FN(&GameLayer::OnKeyPressed));
	}

	bool OnKeyPressed(const Lyra::KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == LR_KEY_F1)
		{
			Lyra::Window& window = Lyra::Application::GetApplication().GetWindow();
			if (window.GetMouseInputMode() == LR_CURSOR_NORMAL)
			{
				window.SetMouseInputMode(LR_CURSOR_DISABLED);
			}
			else if (window.GetMouseInputMode() == LR_CURSOR_DISABLED)
			{
				window.SetMouseInputMode(LR_CURSOR_NORMAL);
			}
			return true;
		}
		return false;
	}

private:
	Ref<Lyra::VertexArray> m_QuadVertexArray;
	Ref<Lyra::VertexArray> m_CubeVertexArray;
	Ref<Lyra::VertexArray> m_LightSourceCubeVertexArray;
	Ref<Lyra::Shader> m_QuadShader;
	Ref<Lyra::Shader> m_TextureShader;
	Ref<Lyra::Shader> m_PhongShader;
	Ref<Lyra::Shader> m_LightSourceShader;

	Ref<Lyra::Texture2D> m_Texture, m_TextureSpecular, m_TransparentTexture;

	Lyra::PerspectiveCameraController m_CameraController;

	float m_CubeRotation;
	float m_CubeRotationSpeed;
	glm::vec3 m_CubePosition;

	float m_ShininessFactor;

	glm::vec3 m_LightPosition;
	glm::vec3 m_LightAmbientColor;
	glm::vec3 m_LightDiffuseColor;
	glm::vec3 m_LightSpecularColor;

	float m_LightSourceAngle;
	float m_LightSourceOrbitRadius;
	float m_LightSourceSpeed;

	bool m_ChangeColor = false;
	float m_Time = 0.0f;
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