#include "Lyra.h"

#include "imgui.h"

class GameLayer : public Lyra::Layer
{
public:
	GameLayer()
		:	Layer("GameLayer"),
			m_CubeRotation(0.0f),
			m_CubeRotationSpeed(12.0f),
			m_CubePosition(glm::vec3(1.5f, 0.0f, 0.0f)),
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
			Ref<Lyra::VertexBuffer> cubeVertexBuffer = Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout);

			m_CubeVertexArray = Lyra::VertexArray::Create();
			m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}
		
		/* LIGHT SOURCE CUBE SECTION */
		{
			m_LightSourceCubeVertexArray = Lyra::VertexArray::Create();

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

			Ref<Lyra::VertexBuffer> cubeVertexBuffer = Lyra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout);
			m_LightSourceCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		}

		// Shader creation
		m_PhongShader = Lyra::Shader::Create("Assets/Shaders/PhongModel.glsl");
		m_LightSourceShader = Lyra::Shader::Create("Assets/Shaders/LightSource.glsl");

		m_Model = Lyra::Model("Assets/Models/sponza/sponza.obj", m_PhongShader);

		// Creating and setting textures
		Lyra::Texture2DProps propsDiffuse("Assets/Textures/Container.png", Lyra::TextureType::DIFFUSE);
		Lyra::Texture2DProps propsSpecular("Assets/Textures/Container_specular.png", Lyra::TextureType::SPECULAR);
		m_BoxTextureDiffuse = Lyra::Texture2D::Create(propsDiffuse);
		m_BoxTextureSpecular = Lyra::Texture2D::Create(propsSpecular);


	}

	void OnAttach() override
	{
		float aspectRatio = Lyra::Application::Get().GetWindow().GetAspectRatio();
		Ref<Lyra::PerspectiveCamera> camera = std::make_shared<Lyra::PerspectiveCamera>(aspectRatio);

		m_CameraController.AttachToCamera(camera);

		Lyra::SceneProps sceneProps;
		sceneProps.camera = camera;

		// Setting up point lights initial values.
		{
			sceneProps.pointLights[0].diffuse = glm::vec3(1.0f, 0.3f, 0.2f);
			sceneProps.pointLights[0].specular = sceneProps.pointLights[0].diffuse;
			sceneProps.pointLights[1].diffuse = glm::vec3(0.2f, 1.0f, 0.3f);
			sceneProps.pointLights[1].specular = sceneProps.pointLights[1].diffuse;
		}

		m_Scene = std::make_shared<Lyra::Scene>(sceneProps);
	}

	void OnDetach() override
	{
		//LR_TRACE("GameLayer::OnDetach");
	}

	void OnUpdate(Lyra::Timestep ts) override
	{
		m_Time += ts;
		m_CameraController.OnUpdate(ts);

		auto& pointLights = m_Scene->GetPointLights();
		glm::vec3 orbitCenter = glm::vec3(0.0f, 0.0f, -1.0f);
		/* Light source orbit */
		m_LightSourceAngle += m_LightSourceSpeed * ts.GetSeconds();
		m_LightSourceAngle = glm::mod(m_LightSourceAngle, glm::two_pi<float>());
		pointLights[0].position = orbitCenter + glm::vec3(
			0.0f,
			4.0f * glm::cos(m_LightSourceAngle),
			4.0f * glm::sin(m_LightSourceAngle)
		);
		m_LightSourceAngle += m_LightSourceSpeed * ts.GetSeconds();
		m_LightSourceAngle = glm::mod(m_LightSourceAngle, glm::two_pi<float>());
		pointLights[1].position = orbitCenter + glm::vec3(
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

		/* Set the scene variables */
		Lyra::Renderer::BeginScene(m_Scene);

		/* Actual rendering happens here */
			
		/* Render point light sources (Non-indexed) */
		//m_LightSourceShader->Bind();
		//for (int i = 0; i < m_SceneProps->pointLights.size(); i++)
		//{
		//	m_LightSourceShader->UploadUniform_3f("u_Color", m_SceneProps->pointLights[i].diffuse);
		//	Lyra::Renderer::Submit(
		//		m_LightSourceShader,
		//		m_LightSourceCubeVertexArray,
		//		glm::translate(glm::mat4(1.0f), m_SceneProps->pointLights[i].position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
		//		false
		//	);

		//}

		/* Render 3D model */
		m_Model.Draw();

		/* Render main cube (Non-indexed) */
		//m_BoxTextureDiffuse->Bind();
		//m_BoxTextureSpecular->Bind();
		//Lyra::Renderer::Submit(
		//	m_PhongShader,
		//	m_CubeVertexArray,
		//	glm::translate(glm::mat4(1.0f), m_CubePosition) * glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))),
		//	false
		//);

		Lyra::Renderer::EndScene();

		m_LastFrameTime = ts.GetSeconds();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Properties");
		if (ImGui::CollapsingHeader("Dir Light"))
		{
			ImGui::Indent();

			ImGui::DragFloat3("Direction", &m_Scene->GetDirectionalLight().direction.x, 0.1f);
			ImGui::ColorEdit3("Ambient##DirLight", &m_Scene->GetDirectionalLight().ambient.x);
			ImGui::ColorEdit3("Diffuse##DirLight", &m_Scene->GetDirectionalLight().diffuse.x);
			ImGui::ColorEdit3("Specular##DirLight", &m_Scene->GetDirectionalLight().specular.x);

			ImGui::Unindent();
		}

		for (int i = 0; i < m_Scene->GetPointLights().size(); ++i)
		{
			ImGui::PushID(i);
			if (ImGui::CollapsingHeader(("Point Light " + std::to_string(i)).c_str()))
			{
				ImGui::Indent();

				ImGui::DragFloat3("Position", &m_Scene->GetPointLights()[i].position.x, 0.1f);
				ImGui::ColorEdit3("Ambient", &m_Scene->GetPointLights()[i].ambient.r);
				ImGui::ColorEdit3("Diffuse", &m_Scene->GetPointLights()[i].diffuse.r);
				ImGui::ColorEdit3("Specular", &m_Scene->GetPointLights()[i].specular.r);
				ImGui::DragFloat("Constant Attenuation", &m_Scene->GetPointLights()[i].constAttenuation, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Linear Attenuation", &m_Scene->GetPointLights()[i].linearAttenuation, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Quadratic Attenuation", &m_Scene->GetPointLights()[i].quadAttenuation, 0.01f, 0.0f, 1.0f);

				ImGui::Unindent();
			}

			ImGui::PopID();
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Flash Light"))
		{
			ImGui::SliderFloat("Inner Ang", &m_Scene->GetSpotLight().innerCutoffAngle, 0.0f, m_Scene->GetSpotLight().outerCutoffAngle);
			ImGui::SliderFloat("Outer Ang", &m_Scene->GetSpotLight().outerCutoffAngle, m_Scene->GetSpotLight().innerCutoffAngle, 90.0f);
			ImGui::ColorEdit3("Ambient##FlashLight", &m_Scene->GetSpotLight().ambient.x);
			ImGui::ColorEdit3("Diffuse##FlashLight", &m_Scene->GetSpotLight().diffuse.x);
			ImGui::ColorEdit3("Specular##FlashLight", &m_Scene->GetSpotLight().specular.x);
			ImGui::DragFloat("Constant Attenuation##FlashLight", &m_Scene->GetSpotLight().constAttenuation, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Linear Attenuation##FlashLight", &m_Scene->GetSpotLight().linearAttenuation, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Quadratic Attenuation##FlashLight", &m_Scene->GetSpotLight().quadAttenuation, 0.01f, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("3D Model"))
		{
			ImGui::Indent();
			auto& materials = m_Model.GetMaterials();
			ImGui::Text(std::format("Materials: {}",  materials.size()).c_str());
			ImGui::Text(std::format("Meshes: {}", m_Model.GetMeshCount()).c_str());
			int materialCount = 0;
			for (auto& [materialHash, material] : materials)
			{
				ImGui::PushID(materialHash);

				if (ImGui::CollapsingHeader(("Material " + std::to_string(materialCount)).c_str())) 
				{
					ImGui::Indent();

					for (auto& texture : material->GetTextures())
					{
						ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.15f, 1.0f), texture->GetTypeAsString());
						ImGui::Image(texture->GetRendererId(), ImVec2(85.f, 85.f));
						ImGui::Text(texture->GetPath().c_str());
						ImGui::Separator();
					}

					ImGui::Unindent();
				}
				materialCount++;
				ImGui::PopID();
			}
			ImGui::Unindent();
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
		ImGui::Value("X", m_CameraController.GetCamera()->GetPosition().x);
		ImGui::SameLine();
		ImGui::Value("Y", m_CameraController.GetCamera()->GetPosition().y);
		ImGui::SameLine();
		ImGui::Value("Z", m_CameraController.GetCamera()->GetPosition().z);
		ImGui::Separator();

		ImGui::SeparatorText("Performance Stats");
		ImGui::Value("Draw Calls", Lyra::Renderer::GetDrawCallCount());
		ImGui::Value("FPS", 1.0f / m_LastFrameTime);
		ImGui::Value("Frametime", m_LastFrameTime);
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
			Lyra::Window& window = Lyra::Application::Get().GetWindow();
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
	Ref<Lyra::Shader> m_LightSourceShader;

	Ref<Lyra::Texture2D> m_BoxTextureDiffuse, m_BoxTextureSpecular;
	float m_CubeRotation;
	float m_CubeRotationSpeed;
	glm::vec3 m_CubePosition;
	float m_CubeShininess;

	Ref<Lyra::Scene> m_Scene;
	float m_LightSourceAngle;
	float m_LightSourceSpeed;

	float m_LastFrameTime = 0.0f;
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