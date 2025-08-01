#pragma once

#include <glm/glm.hpp>

#include "Core/Ref.h"
#include "Renderer/Buffer.h"

namespace Lyra
{
	struct RendererSpecification
	{
		bool useFrameBuffer = true;
	};

	class RendererAPI
	{

	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

	public:
		virtual void Init(const RendererSpecification& spec) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawVertices(const VertexArray* vertexArray, bool drawIndexed = true) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}