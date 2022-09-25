#pragma once

#ifdef CPHI_WINDOWS_PLATFORM

// To be defined by Client
extern Comphi::Application* Comphi::CreateApplication();

int main(int argc, char** argv) {
	
	printf("~ ~ ~ c o m p h i ~ ~ ~\n");
		
	Comphi::Log::Init();
	auto app = Comphi::CreateApplication();
	try {
		app->Run(); 
	}
	catch (std::exception& e) {
		COMPHILOG_CORE_ERROR(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

#endif // CPHI_WINDOWS_PLATFORM

