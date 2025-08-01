#include "EditorLayer.h"

namespace Lyra
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"),
		m_LightSourceAngle(0.0f),
		m_LightSourceSpeed(1.0f),
		m_Window(&Application::Get().GetWindow())
	{
		/* LIGHT SOURCE CUBE SECTION */
		{
			m_LightSourceCubeVertexArray = Scope<VertexArray>(VertexArray::Create());

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

//			VertexLayout cubeVertexLayout
//			{
//				{"a_Position", ShaderData::Float3}
//			};
//
//			VertexBuffer* cubeVertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout);
//			m_LightSourceCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);

		}

		ShaderLibrary::Load("LightSourceShader", "Assets/Shaders/LightSource.glsl");

		ModelProps propsInverted;
		propsInverted.textureFlipOverride = true;
		m_BackpackModel = ModelLibrary::Load("Assets/Models/backpack/backpack.obj", propsInverted);
		m_SponzaModel = ModelLibrary::Load("Assets/Models/sponza/sponza.obj");
		
		const std::vector<std::filesystem::path> skyboxTextures = 
		{
			"Assets/Textures/Skybox/right.jpg",
			"Assets/Textures/Skybox/left.jpg",
			"Assets/Textures/Skybox/top.jpg",
			"Assets/Textures/Skybox/bottom.jpg",
			"Assets/Textures/Skybox/front.jpg",
			"Assets/Textures/Skybox/back.jpg",
		};
		m_Skybox = std::make_shared<Skybox>(skyboxTextures);
	}

	void EditorLayer::OnAttach()
	{
		// Creating a camera and setting up the camera controller
		float aspectRatio = m_Window->GetAspectRatio();
		Ref<PerspectiveCamera> camera = std::make_shared<PerspectiveCamera>(aspectRatio);

		m_CameraController.AttachToCamera(camera);
		m_CameraController.GetCamera()->SetPosition(glm::vec3(0.0f, 1.5f, 12.0f));

		// Creating scene instance
		SceneProps sceneProps;
		sceneProps.camera = camera;
		{
			sceneProps.pointLights[0].diffuse = glm::vec3(1.0f, 0.3f, 0.2f);
			sceneProps.pointLights[0].specular = sceneProps.pointLights[0].diffuse;
			sceneProps.pointLights[1].diffuse = glm::vec3(0.2f, 1.0f, 0.3f);
			sceneProps.pointLights[1].specular = sceneProps.pointLights[1].diffuse;
		}
		m_Scene = std::make_shared<Scene>(sceneProps);

		// Creating first two entities and setting their properties!
		m_SponzaObj = std::make_shared<Entity>();
		m_SponzaObj->SetModel(m_SponzaModel);
		m_SponzaObj->SetScale(0.01f);
		m_SponzaObj->SetYaw(90.0f);

		m_BackpackObj = std::make_shared<Entity>();
		m_BackpackObj->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
		m_BackpackObj->SetScale(0.5f);
		m_BackpackObj->SetRoll(45.0f);
		m_BackpackObj->SetModel(m_BackpackModel);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_Time += ts;

		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}

		/* Set the scene variables */
		Renderer::BeginFrame(m_Scene);

		/* Render entities */
		m_Skybox->Draw();
		m_SponzaObj->Draw();
		m_BackpackObj->Draw();

		Renderer::EndFrame();

		m_LastFrameTime = ts.GetSeconds();
	}

	void EditorLayer::SetupDockspace()
	{
		/* DOCKING SETUP */
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		bool dockOpen = true;
		ImGui::Begin("DockSpace Demo", &dockOpen, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Exit")) { Application::Get().Quit(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void EditorLayer::OnImGuiRender()
	{
		SetupDockspace();

		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
		
		m_ViewportHovered = ImGui::IsWindowHovered();
		if (m_ViewportFocused != ImGui::IsWindowFocused() && ImGui::IsWindowFocused())
		{
			m_ViewportFocused = ImGui::IsWindowFocused();
			SetEditorInputMode(EDITOR_INPUT_MODE::VIEWPORT_MODE);
		}

		ImVec2 availRegion = ImGui::GetContentRegionAvail();
		glm::vec2 newViewportSize = { availRegion.x, availRegion.y };
		if (m_ViewportSize != newViewportSize && newViewportSize.x > 0.0f && newViewportSize.y > 0.0f)
		{
			Renderer::GetFrameBuffer()->Resize(static_cast<uint16_t>(newViewportSize.x), static_cast<uint16_t>(newViewportSize.y));
			m_ViewportSize = { newViewportSize.x, newViewportSize.y };
		}

		ImGui::Image(Renderer::GetFrameBuffer()->GetColorTextureTarget(), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();

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
			auto& meshes = m_SponzaModel->GetMeshes();
			ImGui::Text((std::string("Mesh count: ") + std::to_string(meshes.size())).c_str());
			for (auto& mesh : meshes)
			{
				ImGui::PushID((int)mesh->GetHash());

				if (ImGui::CollapsingHeader(mesh->GetName().data()))
				{
					ImGui::Indent();
					std::string verticesLabel = std::string("Vertices: ") + std::to_string(mesh->GetVertexCount());
					ImGui::SeparatorText(verticesLabel.c_str());
					for (auto& texture : mesh->GetMaterial()->GetTextures())
					{
						//ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.15f, 1.0f), texture->);
						ImGui::Image(texture->GetRendererId(), ImVec2(85.f, 85.f));
						ImGui::Text(texture->GetPath().string().c_str());
						ImGui::Separator();
					}

					ImGui::Unindent();
				}
				ImGui::PopID();
			}
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
		ImGui::Value("Draw Calls", Renderer::GetDrawCallCount());
		ImGui::Value("FPS", 1.0f / m_LastFrameTime);
		ImGui::Value("Frametime", m_LastFrameTime);
		ImGui::Separator();

		ImGui::End();

		ImGui::ShowDemoWindow();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(LR_BIND_EVENT_FN(&EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(const KeyPressedEvent& e)
	{
		// Unfocus the viewport if the key is pressed.
		if (e.GetKeyCode() == VIEWPORT_FOCUS_TOGGLE_KEY)
		{
			ImGui::SetWindowFocus(nullptr);
			m_ViewportFocused = false;
			SetEditorInputMode(EDITOR_INPUT_MODE::EDITING_MODE);
			return true;
		}
		return false;
	}

	void EditorLayer::SetEditorInputMode(EDITOR_INPUT_MODE inputMode)
	{
		switch (inputMode)
		{
		case EDITOR_INPUT_MODE::VIEWPORT_MODE:
		{
			m_Window->SetMouseInputMode(LR_CURSOR_DISABLED);
			Application::Get().GetImGuiLayer()->BlockEvents(false);
			break;
		}
		case EDITOR_INPUT_MODE::EDITING_MODE:
		{
			m_Window->SetMouseInputMode(LR_CURSOR_NORMAL);
			Application::Get().GetImGuiLayer()->BlockEvents(true);
			break;
		}
		default:
			break;
		}
	}
}