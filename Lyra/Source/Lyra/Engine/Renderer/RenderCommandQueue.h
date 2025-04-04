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
		RenderCommandQueue()
		{
			m_CommandQueue.reserve(512);
		}
		~RenderCommandQueue() = default;

		template<class ...Args>
		inline void Enqueue(Args && ...commandArguments)
		{
			// TODO: Sort as I push elements in or sort later? HMM
			// Note for future self:
			//		The plan is to have multiple queues or 'buckets' that can be sorted differently.
			//		For example, a transparent bucket would sort objects from back to front (based on distance) and render them in that order
			//		An opaque bucket would sort objects based on material (group same materials together)
			// Then the flush each queue so make different "passes"
			m_CommandQueue.emplace_back(commandArguments...);
		}

		void Flush()
		{
			for (RenderCommand& command : m_CommandQueue)
			{
				command.Execute();
			}

			m_CommandQueue.clear();
		}

	private:

		std::vector<RenderCommand> m_CommandQueue;
	};
}
