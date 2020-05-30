#include "cphipch.h"
#include "Application.h"

#include "Comphi/Events/ApplicationEvent.h"
#include "Comphi/Log.h"

namespace Comphi {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizedEvent e(1280, 720);
		LOG_CPHY_CORE_INFO(e);

		while (true);
	}

}