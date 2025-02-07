#pragma once

#include "Lyra/Renderer/RendererAPI.h"

namespace Lyra
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		/* Named after DirectX */
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void UploadUniform_Mat4f(uint32_t rendererId, const std::string& name, const glm::mat4& value) override;


	};
}