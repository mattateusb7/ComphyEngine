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

	private:
		float m_time = 0.0f;
	};
}
