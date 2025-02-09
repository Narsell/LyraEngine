#pragma once

#include "Lyra/Renderer/Shader.h"

namespace Lyra
{

	class LYRA_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix) override;

	private:
		uint32_t m_RendererId;
	};

}