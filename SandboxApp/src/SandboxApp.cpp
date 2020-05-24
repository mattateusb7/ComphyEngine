#include <Comphi.h>

class Sandbox : public Comphi::Application
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

Comphi::Application* Comphi::CreateApplication() {
	return new Sandbox();
}