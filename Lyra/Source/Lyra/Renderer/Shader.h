#pragma once

#include "Lyra/Core.h"

#include <string>

namespace Lyra
{

	class LYRA_API Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();

	private:

		std::uint32_t m_RendererId = 0;
	};

}