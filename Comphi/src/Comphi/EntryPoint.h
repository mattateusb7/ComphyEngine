#pragma once

#ifdef CPHI_WINDOWS_PLATFORM

#include "Comphi/Log.h"

// To be defined by Client
extern Comphi::Application* Comphi::CreateApplication();

int main(int argc, char** argv) {
	
	printf(" _^___________^_\n");
	printf("|~ CPHI_Engine ~|\n");
	printf(" `-------------´\n");
	printf("  |0|||||||||0|\n");
	printf("  |^|||/ \\|||^|\n");
	printf("´´´´´´´´´´´´´´´´´\n");
		
	Comphi::Log::Init();
	auto app = Comphi::CreateApplication(); 
	app->Run(); 
	delete app;
}

#endif // CPHI_WINDOWS_PLATFORM

