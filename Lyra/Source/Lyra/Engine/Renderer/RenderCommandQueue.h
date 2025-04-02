#pragma once

#include <cstdint>
#include <vector>

#include "Renderer/RenderCommand.h"
#include "Scene/Scene.h"

namespace Lyra
{
	class RenderCommandQueue
	{
	public:
		RenderCommandQueue();
		~RenderCommandQueue() = default;

		void Enqueue(RenderCommand command);
		void Flush(const SceneProps& sceneProps);

	private:
		size_t m_LastBoundMaterialHash = 0;
		size_t m_LastBoundShaderHash = 0;
		std::vector<RenderCommand> m_CommandQueue;
	};

}
