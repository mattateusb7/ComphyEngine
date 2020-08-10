#pragma once
#include "Comphi/Core.h"
#include "Comphi/Events/Event.h"

namespace Comphi {
	class COMPHI_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_DebugName; }
		inline const bool& GetIsEnabled() const { return m_Enabled; }
	protected:
		std::string m_DebugName;
		bool m_Enabled;
	};
}
