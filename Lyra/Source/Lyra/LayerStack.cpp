#include "lrpch.h"

#include "LayerStack.h"

namespace Lyra
{
	LayerStack::LayerStack()
		: m_LayerInsert(m_Layers.begin())
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		LayerContainer::iterator it = std::find(begin(), m_LayerInsert, layer);
		if (it != end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		LayerContainer::iterator it = std::find(m_LayerInsert, end(), overlay);
		if (it != end())
		{
			m_Layers.erase(it);
		}
	}
}