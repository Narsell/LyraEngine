#pragma once

#include "Lyra/Core/Ref.h"
#include "Lyra/Renderer/RendererAPI.h"

namespace Lyra
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		/* Named after DirectX */
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};
}