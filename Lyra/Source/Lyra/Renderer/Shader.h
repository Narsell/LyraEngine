#pragma once

#include "Lyra/Core/Core.h"

#include <glm/glm.hpp>

namespace Lyra
{

	class LYRA_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void UploadUniform_1i(const std::string& name, int value) = 0;

		virtual void UploadUniform_Mat4f(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void UploadUniform_4f(const std::string& name, const glm::vec4& value) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};

}