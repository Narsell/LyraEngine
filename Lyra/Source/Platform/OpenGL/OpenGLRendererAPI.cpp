#include "lrpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Lyra
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::UploadUniform_Mat4f(uint32_t rendererId, const std::string& name, const glm::mat4& value)
	{
		/* TODO: Cache the uniform location in a map */
		int location = glGetUniformLocation(rendererId, name.c_str());
		if (location == -1)
		{
			LR_CORE_ASSERT(false, "Unable to set shader uniform.");
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}
}