#include "lrpch.h"

#include "LayerStack.h"
#include "Layer.h"

namespace Lyra
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		LayerContainer::iterator it = std::find(begin(), begin() + m_LayerInsertIndex, layer);
		if (it != end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		LayerContainer::iterator it = std::find(begin() + m_LayerInsertIndex, end(), overlay);
		if (it != end())
		{
			m_Layers.erase(it);
		}
	}
}