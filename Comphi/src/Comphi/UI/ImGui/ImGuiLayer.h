#pragma once
#include "Comphi/Core.h"
#include "Comphi/Layer.h"

namespace Comphi{
	class COMPHI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnUpdate() override;
		void OnEvent(Comphi::Event& e) override;
		void OnAttach() override;
		void OnDetach() override;

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) override;
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) override;
		bool OnMouseMovedEvent(MouseMovedEvent& e) override;
		bool OnMouseScrolledEvent(MouseScrolledEvent& e) override;
		bool OnKeyPressedEvent(KeyPressedEvent& e) override;
		bool OnKeyReleasedEvent(KeyReleasedEvent& e) override;
		bool OnKeyTypedEvent(KeyTypedEvent& e) override;
		bool OnWindowResizedEvent(WindowResizedEvent& e) override;
	private:
		float m_time = 0.0f;
	};
}
