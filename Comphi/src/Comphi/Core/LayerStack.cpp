#include "cphipch.h"
#include "LayerStack.h"

namespace Comphi {

	LayerStack::LayerStack() 
	{
	}

	LayerStack::~LayerStack() 
	{
		//for (Layer* Layer : m_Layers) delete Layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin()+m_LayerInsertIndex++, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != end()) {
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(begin(), end(), overlay);
		if (it != end()) {
			m_Layers.erase(it);
		}

	}

}
