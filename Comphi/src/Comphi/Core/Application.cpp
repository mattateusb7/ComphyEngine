#include "cphipch.h"
#include "Application.h"

namespace Comphi {

	std::unique_ptr<Application> Application::s_instance;

	Application::Application()
	{
		//init Singleton
		COMPHILOG_CORE_ASSERT(!s_instance, "Application Already Exists!");
		s_instance.reset(this);

		//INIT WINDOW & EventCallback
		m_Window = IWindow::Create(windowProperties);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		//INIT IMGUI LAYER //TODO: temp ? (application may not want a default Imgui Overlay Layer)
		m_ImGuiLayer = ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		m_Window->Shutdown();
	}

	void Application::Run()
	{
		//Awake Loop
		for (auto layer : m_LayerStack) {
			layer->OnStart();
		}

		while (m_running) {

			//Draw Loop
			m_Window->OnBeginUpdate(*m_sceneGraph);
			
			//Action Loop
			for (auto layer : m_LayerStack) {
				layer->OnUpdate();
			}
			
			//UI Render Loop
			//m_ImGuiLayer->Begin();
			//for (auto layer : m_LayerStack) {
			//	layer->OnUIRender();
			//}
			//m_ImGuiLayer->End();
		
			//Event Loop
			m_Window->OnUpdate();
		};

		//Destroy Loop
		for (auto layer : m_LayerStack) {
			layer->OnEnd();
		}
	}

	void Application::OnEvent(Event& e)
	{

		EventHandler::Bind<WindowCloseEvent>(e, BIND_EVENT_FN(Application::OnWindowClose));
		EventHandler::Bind<WindowResizedEvent>(e, BIND_EVENT_FN(Application::OnWindowResized));
		EventHandler::Bind<FramebufferResizedEvent>(e, BIND_EVENT_FN(Application::OnFramebufferResized));

		//Call Layer Events Handling
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	void Application::PushScene(SceneGraphPtr& scene)
	{
		m_sceneGraph = &scene;
	}

	void Application::PopScene(SceneGraphPtr& scene)
	{
		//auto it = std::find(m_sceneGraph.begin(), m_sceneGraph.end(), scene);
		//if (it != m_sceneGraph.end()) {
		//	m_sceneGraph.erase(it);
		//}
	}

	void Application::PushLayer(Layer& layer)
	{
		m_LayerStack.PushLayer(&layer);
		layer.OnAttach();
	}

	void Application::PushOverlay(Layer& overlay)
	{
		m_LayerStack.PushOverlay(&overlay);
		overlay.OnAttach();
	}

	void Application::PopLayer(Layer& layer)
	{
		m_LayerStack.PopLayer(&layer);
		layer.OnDetach();
	}

	void Application::PopOverlay(Layer& overlay)
	{
		m_LayerStack.PopOverlay(&overlay);
		overlay.OnDetach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true; //Return "event was handled"
	}

	bool Application::OnWindowResized(WindowResizedEvent& e)
	{
		m_Window->OnWindowResized(e.GetOffsetX(), e.GetOffsetY());
		return false;
	}

	bool Application::OnFramebufferResized(FramebufferResizedEvent& e)
	{
		m_Window->OnFramebufferResized(e.GetOffsetX(), e.GetOffsetY());
		return false;
	}


}
