#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
	struct DirectionalLight
	{
		glm::vec3 direction = glm::vec3(0.0, -0.5f, -1.0f);

		glm::vec3 ambient	= glm::vec3(glm::vec3(0.1f, 0.1f, 0.1f));
		glm::vec3 diffuse	= glm::vec3(1.0f);
		glm::vec3 specular  = glm::vec3(1.0f);
	};

	struct PointLight
	{
		glm::vec3 position      = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 ambient       = glm::vec3(glm::vec3(0.1f, 0.1f, 0.1f));
		glm::vec3 diffuse       = glm::vec3(1.0f);
		glm::vec3 specular      = glm::vec3(1.0f);

		float constAttenuation  = 1.0f;
		float linearAttenuation = 0.09f;
		float quadAttenuation   = 0.032f;
	};

	struct SpotLight
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

		float innerCutoffAngle  = 12.5f;
		float outerCutoffAngle  = 17.5f;

		glm::vec3 ambient		= glm::vec3(glm::vec3(0.1f, 0.1f, 0.1f));
		glm::vec3 diffuse		= glm::vec3(1.0f);
		glm::vec3 specular		= glm::vec3(1.0f);

		float constAttenuation  = 1.0f;
		float linearAttenuation = 0.05f;
		float quadAttenuation   = 0.032f;
	};


public:
	GameLayer()
		:	Layer("GameLayer"),
			m_CubeRotation(0.0f),
			m_CubeRotationSpeed(12.0f),
			m_CubePosition(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_CubeShininess(32.f),
			m_LightSourceAngle(0.0f),
			m_LightSourceSpeed(1.0f)
	{ 
	
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
		m_PhongShader = Lyra::Shader::Create("Assets/Shaders/PhongModel.glsl");
		m_ModelShader = Lyra::Shader::Create("Assets/Shaders/Model.glsl");
		m_LightSourceShader = Lyra::Shader::Create("Assets/Shaders/LightSource.glsl");

		m_Model = Lyra::Model("Assets/Models/backpack/backpack.obj", m_ModelShader);

		// Creating and setting textures
		//m_BoxTexture = Lyra::Texture2D::Create("Assets/Textures/Container.png");
		//m_BoxTextureSpecular = Lyra::Texture2D::Create("Assets/Textures/Container_specular.png");
		//m_Texture->Bind(0);
		//m_TextureSpecular->Bind(1);

		// Setting up point lights initial values.
		{
			m_PointLights[0].diffuse = glm::vec3(1.0f, 0.3f, 0.2f);
			m_PointLights[0].specular = m_PointLights[0].diffuse;
			m_PointLights[1].diffuse = glm::vec3(0.2f, 1.0f, 0.3f);
			m_PointLights[1].specular = m_PointLights[1].diffuse;
		}
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

		glm::vec3 orbitCenter = glm::vec3(0.0f, 0.0f, -1.0f);
		/* Light source orbit */
		m_LightSourceAngle += m_LightSourceSpeed * ts.GetSeconds();
		m_LightSourceAngle = glm::mod(m_LightSourceAngle, glm::two_pi<float>());
		m_PointLights[0].position = orbitCenter + glm::vec3(
			0.0f,
			4.0f * glm::cos(m_LightSourceAngle),
			4.0f * glm::sin(m_LightSourceAngle)
		);
		m_LightSourceAngle += m_LightSourceSpeed * ts.GetSeconds();
		m_LightSourceAngle = glm::mod(m_LightSourceAngle, glm::two_pi<float>());
		m_PointLights[1].position = orbitCenter + glm::vec3(
			5.0f * glm::sin(m_LightSourceAngle),
			0.0f,
			5.0f * glm::cos(m_LightSourceAngle)
		);

		
		/* Cube rotation */
		m_CubeRotation += m_CubeRotationSpeed * ts.GetSeconds();
		m_CubeRotation = glm::mod(m_CubeRotation, 360.f);

		/* Clearing buffers */
		Lyra::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Lyra::RenderCommand::Clear();

		// Set the scene variables (only camera at this point)
		Lyra::Renderer::BeginScene(m_CameraController.GetCamera());


		/* Actual rendering happens here */
			
		/* Render point light sources (Non-indexed) */
		m_LightSourceShader->Bind();
		for (int i = 0; i < m_PointLights.size(); i++)
		{
			m_LightSourceShader->UploadUniform_3f("u_Color", m_PointLights[i].diffuse);
			Lyra::Renderer::Submit(
				m_LightSourceShader,
				m_LightSourceCubeVertexArray,
				glm::translate(glm::mat4(1.0f), m_PointLights[i].position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
				false
			);

		}

		/* Render main cube (Non-indexed) */
		//m_PhongShader->Bind();
		//Lyra::Renderer::Submit(
		//	m_PhongShader,
		//	m_CubeVertexArray,
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))) * glm::translate(glm::mat4(1.0f), m_CubePosition),
		//	false
		//);

		m_ModelShader->Bind();
		m_Model.Draw();

		/* Directional light uniforms */
		m_ModelShader->UploadUniform_3f("u_DirLight.direction", glm::mat3(m_CameraController.GetCamera().GetViewMatrix()) * m_DirLight.direction);
		m_ModelShader->UploadUniform_3f("u_DirLight.ambient", m_DirLight.ambient);
		m_ModelShader->UploadUniform_3f("u_DirLight.diffuse", m_DirLight.diffuse);
		m_ModelShader->UploadUniform_3f("u_DirLight.specular", m_DirLight.specular);

		/* Point light uniforms*/
		for (int i = 0; i < m_PointLights.size(); i++)
		{
			m_ModelShader->UploadUniform_3f(std::format("u_PointLights[{0}].position", i), glm::vec3(m_CameraController.GetCamera().GetViewMatrix() * glm::vec4(m_PointLights[i].position, 1.0f)));
			m_ModelShader->UploadUniform_3f(std::format("u_PointLights[{0}].ambient", i), m_PointLights[i].ambient);
			m_ModelShader->UploadUniform_3f(std::format("u_PointLights[{0}].diffuse", i), m_PointLights[i].diffuse);
			m_ModelShader->UploadUniform_3f(std::format("u_PointLights[{0}].specular", i), m_PointLights[i].specular);
			m_ModelShader->UploadUniform_1f(std::format("u_PointLights[{0}].constAttenuation", i), m_PointLights[i].constAttenuation);
			m_ModelShader->UploadUniform_1f(std::format("u_PointLights[{0}].linearAttenuation", i), m_PointLights[i].linearAttenuation);
			m_ModelShader->UploadUniform_1f(std::format("u_PointLights[{0}].quadAttenuation", i), m_PointLights[i].quadAttenuation);
		}

		/* Spot light uniforms */
		m_ModelShader->UploadUniform_3f("u_SpotLight.position", m_SpotLight.position);
		m_ModelShader->UploadUniform_3f("u_SpotLight.direction", m_SpotLight.direction);
		m_ModelShader->UploadUniform_1f("u_SpotLight.innerCutoffCosine", glm::cos(glm::radians(m_SpotLight.innerCutoffAngle)));
		m_ModelShader->UploadUniform_1f("u_SpotLight.outerCutoffCosine", glm::cos(glm::radians(m_SpotLight.outerCutoffAngle)));
		m_ModelShader->UploadUniform_3f("u_SpotLight.ambient", m_SpotLight.ambient);
		m_ModelShader->UploadUniform_3f("u_SpotLight.diffuse", m_SpotLight.diffuse);
		m_ModelShader->UploadUniform_3f("u_SpotLight.specular", m_SpotLight.specular);
		m_ModelShader->UploadUniform_1f("u_SpotLight.constAttenuation", m_SpotLight.constAttenuation);
		m_ModelShader->UploadUniform_1f("u_SpotLight.linearAttenuation", m_SpotLight.linearAttenuation);
		m_ModelShader->UploadUniform_1f("u_SpotLight.quadAttenuation", m_SpotLight.quadAttenuation);

		/* Cube material uniforms */
		//m_PhongShader->Bind();
		//m_PhongShader->UploadUniform_1i("u_Material.diffuse", 0);
		//m_PhongShader->UploadUniform_1i("u_Material.specular", 1);
		//m_PhongShader->UploadUniform_1f("u_Material.shininess", m_ShininessFactor);

		Lyra::Renderer::EndScene();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Properties");
		if (ImGui::CollapsingHeader("Dir Light"))
		{
			ImGui::Indent();

			ImGui::DragFloat3("Direction", &m_DirLight.direction.x, 0.1f);
			ImGui::ColorEdit3("Ambient##DirLight", &m_DirLight.ambient.x);
			ImGui::ColorEdit3("Diffuse##DirLight", &m_DirLight.diffuse.x);
			ImGui::ColorEdit3("Specular##DirLight", &m_DirLight.specular.x);

			ImGui::Unindent();
		}

		for (int i = 0; i < m_PointLights.size(); ++i) {
			// Push a unique ID scope for each light to avoid label conflicts
			ImGui::PushID(i);

			// Create a collapsible header with the light's index
			if (ImGui::CollapsingHeader(("Point Light " + std::to_string(i)).c_str())) {
				ImGui::Indent(); // Indent controls for visual hierarchy

				ImGui::DragFloat3("Position", &m_PointLights[i].position.x, 0.1f);
				ImGui::ColorEdit3("Ambient", &m_PointLights[i].ambient.r);
				ImGui::ColorEdit3("Diffuse", &m_PointLights[i].diffuse.r);
				ImGui::ColorEdit3("Specular", &m_PointLights[i].specular.r);
				ImGui::DragFloat("Constant Attenuation", &m_PointLights[i].constAttenuation, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Linear Attenuation", &m_PointLights[i].linearAttenuation, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Quadratic Attenuation", &m_PointLights[i].quadAttenuation, 0.01f, 0.0f, 1.0f);

				ImGui::Unindent();
			}

			ImGui::PopID(); // End unique ID scope
			ImGui::Separator(); // Visual separation between lights
		}

		if (ImGui::CollapsingHeader("Flash Light"))
		{
			ImGui::SliderFloat("Inner Ang", &m_SpotLight.innerCutoffAngle, 0.0f, m_SpotLight.outerCutoffAngle);
			ImGui::SliderFloat("Outer Ang", &m_SpotLight.outerCutoffAngle, m_SpotLight.innerCutoffAngle, 90.0f);
			ImGui::ColorEdit3("Ambient##FlashLight", &m_SpotLight.ambient.x);
			ImGui::ColorEdit3("Diffuse##FlashLight", &m_SpotLight.diffuse.x);
			ImGui::ColorEdit3("Specular##FlashLight", &m_SpotLight.specular.x);
			ImGui::DragFloat("Constant Attenuation##FlashLight", &m_SpotLight.constAttenuation, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Linear Attenuation##FlashLight", &m_SpotLight.linearAttenuation, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Quadratic Attenuation##FlashLight", &m_SpotLight.quadAttenuation, 0.01f, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Cube"))
		{
			ImGui::Indent();
			ImGui::Text("Cube");
			ImGui::SliderFloat("Shininess", &m_CubeShininess, 2.f, 256.f);
			ImGui::DragFloat3("Cube Position", &m_CubePosition.x, 0.1f);
			ImGui::SliderFloat("Cube Rot Speed", &m_CubeRotationSpeed, 0.0f, 100.0f);
			ImGui::Text("Camera Position:");
			ImGui::Unindent();
		}

		ImGui::SeparatorText("Camera Position");
		ImGui::Value("X", m_CameraController.GetCamera().GetPosition().x);
		ImGui::SameLine();
		ImGui::Value("Y", m_CameraController.GetCamera().GetPosition().y);
		ImGui::SameLine();
		ImGui::Value("Z", m_CameraController.GetCamera().GetPosition().z);
		ImGui::Separator();

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
		/* Toggle mouse input mode so we can use ImGui controls by pressing F1 */
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
	Lyra::PerspectiveCameraController m_CameraController;
	
	Lyra::Model m_Model;
	
	Ref<Lyra::VertexArray> m_CubeVertexArray;
	Ref<Lyra::VertexArray> m_LightSourceCubeVertexArray;
	Ref<Lyra::Shader> m_PhongShader;
	Ref<Lyra::Shader> m_ModelShader;
	Ref<Lyra::Shader> m_LightSourceShader;

	Ref<Lyra::Texture2D> m_BoxTexture, m_BoxTextureSpecular;
	float m_CubeRotation;
	float m_CubeRotationSpeed;
	glm::vec3 m_CubePosition;
	float m_CubeShininess;

	DirectionalLight m_DirLight;
	std::array<PointLight, 2> m_PointLights;
	SpotLight m_SpotLight;
	float m_LightSourceAngle;
	float m_LightSourceSpeed;

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