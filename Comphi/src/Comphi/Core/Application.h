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
		void PushScene(ScenePtr& scene);
		void PopScene(ScenePtr& scene);
		void PushLayer(Layer& layer);
		void PushOverlay(Layer& overlay);
		void PopLayer(Layer& layer);
		void PopOverlay(Layer& overlay);

		inline IWindow& GetWindowHandler() { return *m_Window; };

		inline static Application& Get() { return *s_instance; };

	protected:
		WindowProperties windowProperties;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
		bool OnFramebufferResized(FramebufferResizedEvent& e);
	
		LayerStack m_LayerStack;
		SceneGraph m_sceneGraph;
		IWindow* m_Window;
		ImGuiLayer m_ImGuiLayer;
		bool m_running = true;

		static std::unique_ptr<Application> s_instance;

	};

	// To be defined by Client
	Application* CreateApplication();

}