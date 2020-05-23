#pragma once

#ifdef CPHY_WINDOWS_PLATFORM

extern Comphy::Application* Comphy::CreateApplication();

int main(int argc, char** argv) {
	
	printf("~Be Comphy, with ComphyEngine ~");
	auto app = Comphy::CreateApplication();
	app->Run();
	delete app;
}

#endif // CPHY_WINDOWS_PLATFORM

