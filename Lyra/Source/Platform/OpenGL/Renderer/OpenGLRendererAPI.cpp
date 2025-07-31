#include "lrpch.h"

#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Lyra
{
	void OpenGLRendererAPI::Init(const RendererSpecification& spec)
	{
		// TODO: Expose these params to the API and make them configurable
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// TODO: Should disable/enable this dynamically when dealing with open meshes.
		glEnable(GL_CULL_FACE);
		
		// TODO: Add a flag to enable wireframe view!
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDisable(GL_CULL_FACE);
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

	void OpenGLRendererAPI::DrawVertices(const VertexArray* vertexArray, bool drawIndexed)
	{
		vertexArray->Bind();

		if (drawIndexed)
		{
			DrawIndexed(vertexArray);
		}
		else
		{
			DrawUnindexed(vertexArray);
		}
	}

	void OpenGLRendererAPI::DrawIndexed(const VertexArray* vertexArray)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawUnindexed(const VertexArray* vertexArray)
	{
		for (const auto& vertexBuffer : vertexArray->GetVertexBuffers())
		{
			// For some reason OpenGL receives a GLsizei (typedef int) as a count for vertex count? wut
			// Idk just keep in mind in case this cast (uint32 to int) ever becomes a problem.
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexBuffer->GetVertexCount()));
		}
	}
}