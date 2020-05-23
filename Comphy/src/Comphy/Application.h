#pragma once 

#include "Core.h"

namespace Comphy {

	class COMPHY_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

	};

	// To be defined by Client

	Application* CreateApplication();

}