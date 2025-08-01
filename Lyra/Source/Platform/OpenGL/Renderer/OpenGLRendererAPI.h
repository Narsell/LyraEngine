#pragma once

#include "Core/Ref.h"
#include "Renderer/RendererAPI.h"

namespace Lyra
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init(const RendererSpecification& spec) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawVertices(const VertexArray* vertexArray, bool drawIndexed = true) override;

	private:
		virtual void DrawUnindexed(const VertexArray* vertexArray);
		virtual void DrawIndexed(const VertexArray* vertexArray);

	};
}