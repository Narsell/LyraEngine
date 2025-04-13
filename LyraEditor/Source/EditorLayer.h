#pragma once

#include "Lyra.h"
#include "imgui.h"

namespace Lyra
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer()
			: Layer("EditorLayer"),
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

				VertexLayout cubeVertexLayout
				{
					{"a_Position", ShaderData::Float3},
					{"a_Normal", ShaderData::Float3},
					{"a_TexCoord", ShaderData::Float2}
				};
				cubeVertexLayout.DebugPrint("Phong model cube");
				VertexBuffer* cubeVertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout);

				m_CubeVertexArray = Scope< VertexArray>(VertexArray::Create());
				m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
			}

			/* LIGHT SOURCE CUBE SECTION */
			{
				m_LightSourceCubeVertexArray = Scope< VertexArray>(VertexArray::Create());

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

				VertexLayout cubeVertexLayout
				{
					{"a_Position", ShaderData::Float3}
				};

				VertexBuffer* cubeVertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices), cubeVertexLayout);
				m_LightSourceCubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
			}

			// Set shader references
			m_LightSourceShader = ShaderLibrary::Load("LightSourceShader", "Assets/Shaders/LightSource.glsl");

			ModelProps propsInverted;
			propsInverted.textureFlipOverride = true;
			m_BackpackModel = ModelLibrary::Load("Assets\\Models\\backpack\\backpack.obj", propsInverted);
			m_SponzaModel = ModelLibrary::Load("Assets\\Models\\sponza\\sponza.obj");

			// Creating and setting textures
			Texture2DProps propsDiffuse(TextureType::DIFFUSE);
			Texture2DProps propsSpecular(TextureType::SPECULAR);

			m_BoxTextureDiffuse = TextureLibrary::Load("Assets\\Textures\\Container.png", propsDiffuse);
			m_BoxTextureSpecular = TextureLibrary::Load("Assets\\Textures\\Container_specular.png", propsSpecular);
		}

		void OnAttach() override
		{
			// Creating a camera and setting up the camera controller
			float aspectRatio = Application::Get().GetWindow().GetAspectRatio();
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

		void OnDetach() override
		{
		}

		void OnUpdate(Timestep ts) override
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

			/* Set the scene variables */
			Renderer::BeginFrame(m_Scene);

			/* Actual rendering happens here */

			/* Render point light sources (Non-indexed) */
			//m_LightSourceShader->Bind();
			//for (int i = 0; i < m_SceneProps->pointLights.size(); i++)
			//{
			//	m_LightSourceShader->UploadUniform_3f("u_Color", m_SceneProps->pointLights[i].diffuse);
			//	Renderer::Submit(
			//		m_LightSourceShader,
			//		m_LightSourceCubeVertexArray,
			//		glm::translate(glm::mat4(1.0f), m_SceneProps->pointLights[i].position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
			//		false
			//	);
			//}

			/* Render entities */
			m_SponzaObj->Draw();
			m_BackpackObj->Draw();

			/* Render main cube (Non-indexed) */
			//m_BoxTextureDiffuse->Bind();
			//m_BoxTextureSpecular->Bind();
			//Renderer::Submit(
			//	m_PhongShader,
			//	m_CubeVertexArray,
			//	glm::translate(glm::mat4(1.0f), m_CubePosition) * glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))),
			//	false
			//);

			Renderer::EndFrame();

			m_LastFrameTime = ts.GetSeconds();
		}

		void OnImGuiRender() override
		{
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

			auto& window = Application::Get().GetWindow();
			ImGui::Begin("Viewport");
			ImGui::Image(Renderer::GetFrameBuffer()->GetColorTextureTarget(), ImVec2(850.0, 480.0), ImVec2(0, 1), ImVec2(1, 0));
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
					ImGui::PushID(mesh->GetHash());

					if (ImGui::CollapsingHeader(mesh->GetName().data()))
					{
						ImGui::Indent();
						std::string verticesLabel = std::string("Vertices: ") + std::to_string(mesh->GetVertexCount());
						ImGui::SeparatorText(verticesLabel.c_str());
						for (auto& texture : mesh->GetMaterial()->GetTextures())
						{
							ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.15f, 1.0f), texture->GetTypeAsString());
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
			ImGui::Value("Draw Calls", Renderer::GetDrawCallCount());
			ImGui::Value("FPS", 1.0f / m_LastFrameTime);
			ImGui::Value("Frametime", m_LastFrameTime);
			ImGui::Separator();

			ImGui::End();

			ImGui::ShowDemoWindow();
		}

		void OnEvent(Event& e) override
		{
			m_CameraController.OnEvent(e);

			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<KeyPressedEvent>(LR_BIND_EVENT_FN(&EditorLayer::OnKeyPressed));
		}

		bool OnKeyPressed(const KeyPressedEvent& e)
		{
			/* Toggle mouse input mode so we can use ImGui controls by pressing F1 */
			if (e.GetKeyCode() == LR_KEY_F1)
			{
				Window& window = Application::Get().GetWindow();
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
		PerspectiveCameraController m_CameraController;

		Ref<Model> m_SponzaModel;
		Ref<Model> m_BackpackModel;

		Ref<Entity> m_SponzaObj;
		Ref<Entity> m_BackpackObj;

		Scope<VertexArray> m_CubeVertexArray;
		Scope<VertexArray> m_LightSourceCubeVertexArray;
		Ref<Shader> m_LightSourceShader;

		Ref<Texture2D> m_BoxTextureDiffuse, m_BoxTextureSpecular;
		float m_CubeRotation;
		float m_CubeRotationSpeed;
		glm::vec3 m_CubePosition;
		float m_CubeShininess;

		Ref<Scene> m_Scene;
		float m_LightSourceAngle;
		float m_LightSourceSpeed;

		float m_LastFrameTime = 0.0f;
		float m_Time = 0.0f;
	};

}