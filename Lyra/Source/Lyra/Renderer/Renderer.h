#pragma once

#include "RendererAPI.h"
#include "Lyra/Renderer/Camera.h"
#include "Shader.h"

namespace Lyra
{

	class Renderer
	{
	public:
		static void BeginScene(const Camera& camera, const std::array<std::shared_ptr<Shader>, 2>& shaders);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}