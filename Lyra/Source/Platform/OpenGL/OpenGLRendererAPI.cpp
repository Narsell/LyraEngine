#include "lrpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Lyra
{
	void OpenGLRendererAPI::Init()
	{
		// TODO: Expose these params to the API and make them configurable
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawUnindexed(const Ref<VertexArray>& vertexArray)
	{
		for (const auto& vertexBuffer : vertexArray->GetVertexBuffers())
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->GetVertexCount());
		}
	}
}