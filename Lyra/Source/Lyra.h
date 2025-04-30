#pragma once

// ONLY FOR APPLICATION USE

/* CORE SYSTEMS */
#include "Lyra/Core/Application.h"
#include "Lyra/Core/Timestep.h"
#include "Lyra/Core/Log.h"
#include "Lyra/Core/Ref.h"
#include "Lyra/Core/Utils.h"
#include "Lyra/Core/Entity.h"

/* INPUT SYSTEM */
#include "Lyra/Core/Input/Input.h"
#include "Lyra/Core/Input/KeyCodes.h"
#include "Lyra/Core/Input/MouseButtonCodes.h"

/* EVENT SYSTEM */
#include "Lyra/Core/Events/KeyEvent.h"
#include "Lyra/Core/Events/ApplicationEvent.h"
#include "Lyra/Core/Events/MouseEvent.h"

/* CONTROLLERS */
#include "Lyra/Scene/Camera/OrthographicCameraController.h"
#include "Lyra/Scene/Camera/PerspectiveCameraController.h"

/* LAYER SYSTEM*/
#include "Lyra/Engine/Layer/Layer.h"
#include "Lyra/Engine/Layer/ImGuiLayer.h"

/* RENDERER */
#include "Lyra/Engine/Renderer/Renderer.h"
#include "Lyra/Engine/Renderer/RenderCommand.h"
#include "Lyra/Engine/Renderer/Buffer.h"
#include "Lyra/Engine/Renderer/FrameBuffer.h"
#include "Lyra/Engine/Renderer/VertexLayout.h"
#include "Lyra/Engine/Renderer/Shader.h"

/* ASSET LIBRARIES */
#include "Lyra/Assets/MaterialLibrary.h"
#include "Lyra/Assets/TextureLibrary.h"
#include "Lyra/Assets/ModelLibrary.h"
#include "Lyra/Assets/ShaderLibrary.h"

/* SCENE ASSETS */
#include "Lyra/Scene/Scene.h"
#include "Lyra/Scene/Texture.h"
#include "Lyra/Scene/Model.h"
#include "Lyra/Scene/Camera/Camera.h"
#include "Lyra/Scene/Light.h"

/* MATH */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
