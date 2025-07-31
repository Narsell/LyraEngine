#pragma once

#include "Core/Ref.h"
#include "Renderer/RendererAPI.h"
#include "Assets/Material.h"
#include "Scene/Scene.h"

namespace Lyra
{
	// Note: Not sure if LR_SKYBOX would be a render type.
	// Rendering a skybox doesn't feel fundamentally as diferent to guarentee a different rendering type or mode.
	// It just removes translation from the view matrix and scales transforms proportional to the camera's render distance.
	// But it will do for now.
	enum class RenderType : uint8_t
	{
		LR_OPAQUE = 0,
		LR_TRANSPARENT,
		LR_SKYBOX
	};

	struct RenderCommandData
	{
		const VertexArray* vertexArray = nullptr;
		Ref<Material> material;
		Ref<const Scene> scene;
		glm::mat4 transform = glm::mat4(1.0f);
		bool drawIndexed = true;
		RenderType renderType = RenderType::LR_OPAQUE;
	};

	class RenderCommand
	{
	public:
		RenderCommand(const RenderCommandData& commandData);
		RenderCommand(const VertexArray* vertexArray, 
					  const Ref<Material>& material,
					  const Ref<const Scene>& scene,
					  const glm::mat4& transform,
					  bool drawIndexed = true,
					  RenderType renderType = RenderType::LR_OPAQUE);
		RenderCommand();
		~RenderCommand() = default;

		void Execute();

		const RenderCommandData& GetData() const { return m_CommandData; }

		inline static void InitRenderer(const RendererSpecification& spec) { s_RendererAPI->Init(spec); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color);	}
		inline static void Clear() { s_RendererAPI->Clear(); }

	private:
		void UploadSceneUniforms();
		void UploadMaterialUniforms();
		void UploadEntityUniforms();

	private:
		RenderCommandData m_CommandData;
		size_t m_LastSceneShaderBound = 0;

		static RendererAPI* s_RendererAPI;
	};
}
