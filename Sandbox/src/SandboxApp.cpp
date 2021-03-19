#include <Comphi.h>
#include <imgui.h>

class TestLayer : public Comphi::Layer
{
public:
	TestLayer() : Layer("Test") {
	
	};

	void OnUpdate() override 
	{



	};
		
	void OnImGuiRender() override 
	{
		ImGui::Begin("Test");
		ImGui::Text("HelloWorld");	
		ImGui::End();
	};

	void OnEvent(Comphi::Event& e) override 
	{
		
	};

private:

};

class Sandbox : public Comphi::Application
{
public:
	Sandbox() {
		//PushLayer(new TestLayer());
	}

	~Sandbox(){}

private:

};

Comphi::Application* Comphi::CreateApplication() {
	return new Sandbox();
}