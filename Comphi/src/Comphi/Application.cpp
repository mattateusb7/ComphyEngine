#include "cphipch.h"
#include "Application.h"

namespace Comphi {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		COMPHILOG_CORE_ASSERT(!s_instance, "Application Already Exists!");
		s_instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		Dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(Application::OnWindowResized));

		//set Layer Events Handling
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
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

	void Application::Run()
	{
		while (m_running) {
			
			m_Window->OnBeginUpdate();

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
				
			m_Window->OnUpdate(); 
		};
	}
}