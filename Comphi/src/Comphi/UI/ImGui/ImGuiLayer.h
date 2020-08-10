#pragma once
#include "Comphi/Core.h"
#include "Comphi/Layer.h"

namespace Comphi{
	class COMPHI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnImGuiRender() override;
		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_time = 0.0f;
	};
}
