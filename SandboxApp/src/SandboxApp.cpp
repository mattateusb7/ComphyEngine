#include <Comphy.h>

class Sandbox : public Comphy::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}

private:

};

Comphy::Application* Comphy::CreateApplication() {
	return new Sandbox();
}