#pragma once 
#include "Comphi/Core.h"
#include "Comphi/Window.h"
#include "Comphi/LayerStack.h"

namespace Comphi {

	class CPHI_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);		
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
	
	private:

		bool OnWindowClose(WindowCloseEvent& e);
	
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		bool m_running = true;
	};

	// To be defined by Client
	Application* CreateApplication();

}