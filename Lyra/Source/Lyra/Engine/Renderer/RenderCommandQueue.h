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
		void Flush();

	private:

		std::vector<RenderCommand> m_CommandQueue;
	};

}
