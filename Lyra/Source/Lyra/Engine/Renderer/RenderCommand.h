#pragma once

#include "Core/Ref.h"
#include "Renderer/RendererAPI.h"
#include "Scene/Material.h"
#include "Scene/Scene.h"

namespace Lyra
{
	enum class RenderType : uint8_t
	{
		LR_OPAQUE = 0,
		LR_TRANSPARENT
	};

	struct RenderCommandData
	{
		VertexArray* vertexArray = nullptr;
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
		RenderCommand(VertexArray* vertexArray, const Ref<Material>& material, const Ref<const Scene>& scene, const glm::mat4& transform, bool drawIndexed = true, RenderType renderType = RenderType::LR_OPAQUE);
		~RenderCommand() = default;

		void Execute();

		const RenderCommandData& GetData() const { return m_CommandData; }

		inline static void InitRenderer() { s_RendererAPI->Init(); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color);	}
		inline static void Clear() { s_RendererAPI->Clear(); }

	private:
		void SetSceneUniforms();
		void SetMaterialUniforms();
		void SetEntityUniforms();

	private:
		RenderCommandData m_CommandData;

		static RendererAPI* s_RendererAPI;
	};
}
