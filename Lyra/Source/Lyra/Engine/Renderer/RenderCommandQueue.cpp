#include "lrpch.h"

#include "RenderCommandQueue.h"

namespace Lyra
{
	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandQueue.reserve(512);
	}

	void RenderCommandQueue::Enqueue(RenderCommand command)
	{
		// TODO: Sort as I push elements in or sort later? HMM
		m_CommandQueue.push_back(command);
	}

	void RenderCommandQueue::Flush()
	{
		for (RenderCommand& command : m_CommandQueue)
		{
			command.Execute();
		}

		m_CommandQueue.clear();
	}
}