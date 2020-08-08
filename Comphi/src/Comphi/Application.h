#pragma once 

#include "Core.h"

namespace Comphi {

	class CPHI_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	// To be defined by Client
	Application* CreateApplication();

}