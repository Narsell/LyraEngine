#pragma once

#include "Lyra/Core/Ref.h"
#include "Lyra/Renderer/Camera.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace Lyra
{

	class Renderer
	{
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static glm::mat4 s_ViewProjectionMatrix;
	};
}