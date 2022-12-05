#pragma once
#include "Comphi/Events/Event.h"

namespace Comphi {
	class Layer //Functional Application Layer (not necessarly dedicated to Rendering)
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnStart() {};
		virtual void OnUpdate() {};
		virtual void OnUIRender() {};
		virtual void OnEvent(Event& event) {};
		virtual void OnEnd() {};

		inline const std::string& GetName() const { return m_DebugName; }
		inline const bool& GetIsEnabled() const { return m_Enabled; }
	protected:
		std::string m_DebugName;
		bool m_Enabled;
	};
}
