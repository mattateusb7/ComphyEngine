#pragma once 
#include "Comphi/Core.h"
#include "Comphi/Window.h"
#include "Comphi/Events/ApplicationEvent.h"


namespace Comphi {

	class CPHI_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_running = true;
	};

	// To be defined by Client
	Application* CreateApplication();

}