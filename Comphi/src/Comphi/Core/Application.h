#pragma once 
#include "Comphi/Platform/IWindow.h"

namespace Comphi {

	class Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer& layer);
		void PushOverlay(Layer& overlay);		
		void PopLayer(Layer& layer);
		void PopOverlay(Layer& overlay);
	
		inline IWindow& GetWindow() { return *m_Window; };

		inline static Application& Get() { return *s_instance; };
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
		bool OnFramebufferResized(FramebufferResizedEvent& e);
	
		LayerStack m_LayerStack;
		std::shared_ptr<IWindow> m_Window;
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;
		bool m_running = true;
	private:
		static std::unique_ptr<Application> s_instance;
	};

	// To be defined by Client
	Application* CreateApplication();

}