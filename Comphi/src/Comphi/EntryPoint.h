#pragma once

#ifdef CPHI_WINDOWS_PLATFORM

#include "Comphi/Log.h"

// To be defined by Client
extern Comphi::Application* Comphi::CreateApplication();

int main(int argc, char** argv) {
	

	printf("´´´comphi´´´\n");
		
	Comphi::Log::Init();
	auto app = Comphi::CreateApplication(); 
	app->Run(); 
	delete app;
}

#endif // CPHI_WINDOWS_PLATFORM

