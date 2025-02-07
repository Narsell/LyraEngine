#pragma once

#include "RendererAPI.h"

namespace Lyra
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		/* TODO: Investigate, this should probably be part of the shader... */
		inline static void UploadUniform_Mat4f(uint32_t rendererId, const std::string& name, const glm::mat4& value)
		{
			s_RendererAPI->UploadUniform_Mat4f(rendererId, name, value);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}