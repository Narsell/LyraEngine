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
		static void Init();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), bool drawIndexed = true);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static glm::mat4 s_ViewProjectionMatrix;
	};
}