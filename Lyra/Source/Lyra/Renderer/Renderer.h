#pragma once

#include "RendererAPI.h"
#include "Lyra/Renderer/Camera.h"
#include "Shader.h"

namespace Lyra
{

	class Renderer
	{
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static const glm::mat4& GetVPMatrix() { return s_VPmatrix; }

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static glm::mat4 s_VPmatrix;
	};
}