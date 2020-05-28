#pragma once

#ifdef CPHI_WINDOWS_PLATFORM

#include "Log.h"

extern Comphi::Application* Comphi::CreateApplication();

int main(int argc, char** argv) {
	
	printf("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n");
	printf("~ Be Comphi with CPHI_Engine ~\n");
	printf("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n");
		
	Comphi::Log::Init();
	CPHY_WARN("OWO");
	auto app = Comphi::CreateApplication();
	app->Run();
	delete app;
}

#endif // CPHI_WINDOWS_PLATFORM

