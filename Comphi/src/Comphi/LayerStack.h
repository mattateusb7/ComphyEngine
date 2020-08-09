#pragma once
#include "Comphi/Core.h"
#include "Comphi/Layer.h"

namespace Comphi {
	
	typedef std::vector<Layer*> Layers;
	
	class COMPHI_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		const inline Layers::iterator begin() { return m_Layers.begin(); }
		const inline Layers::iterator end() { return m_Layers.end(); }
	private:
		Layers m_Layers;
		Layers::iterator m_LayerInsert;
	};
}
