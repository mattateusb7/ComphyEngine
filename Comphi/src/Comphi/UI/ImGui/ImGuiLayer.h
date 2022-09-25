#pragma once
#include "Comphi/Core/Layer.h"

namespace Comphi{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnUIRender() override;
		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_time = 0.0f;
	};
}
