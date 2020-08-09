#pragma once
#include "Comphi/Core.h"
#include "Comphi/Events/Event.h"
#include "Comphi/Events/MouseEvent.h"
#include "Comphi/Events/KeyEvent.h"
#include "Comphi/Events/ApplicationEvent.h"

namespace Comphi {
	class COMPHI_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_DebugName; }
		inline const bool& GetIsEnabled() const { return m_Enabled; }
	protected :
		//Event Handling
		virtual bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) { return false; };
		virtual bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) { return false; };
		virtual bool OnMouseMovedEvent(MouseMovedEvent& e) { return false; };
		virtual bool OnMouseScrolledEvent(MouseScrolledEvent& e) { return false; };
		virtual bool OnKeyPressedEvent(KeyPressedEvent& e) { return false; };
		virtual bool OnKeyReleasedEvent(KeyReleasedEvent& e) { return false; };
		virtual bool OnKeyTypedEvent(KeyTypedEvent& e) { return false; };
		virtual bool OnWindowResizedEvent(WindowResizedEvent& e) { return false; };
	protected:
		std::string m_DebugName;
		bool m_Enabled;
	};
}
