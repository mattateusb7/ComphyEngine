#pragma once 
#include "Comphi/Core.h"
#include "Comphi/Window.h"
#include "Comphi/LayerStack.h"

namespace Comphi {

	class COMPHI_API Application
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
	
		inline Window& GetWindow() { return *m_Window; };

		inline static Application& Get() { return *s_instance; };
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		bool m_running = true;
	private:
		static Application* s_instance;
	};

	// To be defined by Client
	Application* CreateApplication();

}