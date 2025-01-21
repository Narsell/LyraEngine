#pragma once

namespace Lyra
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetApi() { return s_RendererApi; }

	private:
		static RendererAPI s_RendererApi;
	};
}