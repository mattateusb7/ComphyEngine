#include "cphipch.h"
#include "Application.h"
#include "Comphi/Platform/IInput.h"

namespace Comphi {

	std::unique_ptr<Application> Application::s_instance;

	Application::Application()
	{
		//init Singleton
		COMPHILOG_CORE_ASSERT(!s_instance, "Application Already Exists!");
		s_instance.reset(this);

		//INIT WINDOW & EventCallback
		m_Window.reset(IWindow::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		//INIT IMGUI
		m_ImGuiLayer = std::make_shared<ImGuiLayer>();
		PushOverlay(*m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_running) {

			m_Window->OnBeginUpdate();

			for (auto layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//m_ImGuiLayer->Begin();
				for (auto layer : m_LayerStack) {
					layer->OnUIRender();
				}
			//m_ImGuiLayer->End();
		
			m_Window->OnUpdate();
		};
	}

	void Application::OnEvent(Event& e)
	{

		EventHandler::Throw<WindowCloseEvent>(e, BIND_EVENT_FN(Application::OnWindowClose));
		EventHandler::Throw<WindowResizedEvent>(e, BIND_EVENT_FN(Application::OnWindowResized));

		//set Layer Events Handling
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
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


}