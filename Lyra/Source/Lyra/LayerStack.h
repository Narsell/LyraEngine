#pragma once

#include <vector>

#include "Lyra/Core.h"
#include "Layer.h"

namespace Lyra
{
	/*
		Wrapper around a Layer container with iterators.
		Layers are stored first and overlays are bunched up at the end of the stack.
		Takes full ownership over the layers. (Tied to LayerStack instance lifetime)
	*/
	class LYRA_API LayerStack
	{
		using LayerContainer = std::vector<Layer*>;

	public:
		LayerStack();
		~LayerStack();

		// Pushes a layer into the layer stack.
		void PushLayer(Layer* layer);
		// Pushes an overlay layer into the layer stack, right after all the layers.
		void PushOverlay(Layer* overlay);
		// Pops the given layer from the layer stack.
		void PopLayer(Layer* layer);
		// Pops the given overlay layer from the layer stack.
		void PopOverlay(Layer* overlay);

		LayerContainer::iterator begin() { return m_Layers.begin(); }
		LayerContainer::iterator end() { return m_Layers.end(); }

		LayerContainer::const_iterator cbegin() const { return m_Layers.cbegin(); }
		LayerContainer::const_iterator cend() const { return m_Layers.cend(); }

		LayerContainer::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		LayerContainer::reverse_iterator rend() { return m_Layers.rend(); }

		LayerContainer::const_reverse_iterator crbegin() { return m_Layers.crbegin(); }
		LayerContainer::const_reverse_iterator crend() { return m_Layers.crend(); }

	private:
		LayerContainer m_Layers;
		LayerContainer::iterator m_LayerInsert;
	};

}