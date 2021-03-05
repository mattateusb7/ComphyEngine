#pragma once 
#include "Comphi/Core.h"
#include "Comphi/LayerStack.h"
#include "Comphi/Platform/IWindow.h"
#include "Comphi/UI/ImGui/ImGuiLayer.h"

namespace Comphi {

	class Application
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
	
		inline IWindow& GetWindow() { return *m_Window; };

		inline static Application& Get() { return *s_instance; };
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	
		LayerStack m_LayerStack;
		std::unique_ptr<IWindow> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
	private:
		static Application* s_instance;
	};

	// To be defined by Client
	Application* CreateApplication();

}