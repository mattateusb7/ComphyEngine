#pragma once
#include "Layer.h"
#include "Comphi/Events/ApplicationEvent.h"
#include "Comphi/Events/MouseEvent.h"
#include "Comphi/Events/KeyEvent.h"

namespace Comphi {
	
	typedef std::vector<Layer*> Layers;
	
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer	(Layer* layer);
		void PopLayer	(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay	(Layer* overlay);

		const inline Layers::iterator begin() { return m_Layers.begin(); }
		const inline Layers::iterator end() { return m_Layers.end(); }
	private:
		Layers m_Layers;
		uint m_LayerInsertIndex = 0;
	};
}
