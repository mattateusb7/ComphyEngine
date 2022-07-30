#include "cphipch.h"
#include "Input.h"
#include "Comphi/Core/Application.h"

std::unique_ptr<Comphi::IInput> Comphi::IInput::s_instance = std::make_unique<Comphi::Windows::Input>();

namespace Comphi::Windows {
	
	bool Input::IsKeyPressedImpl(int keycode)
	{
		//auto window = std::static_pointer_cast<GLFWwindow>((Application::Get().GetWindow().GetNativeWindow()));
		auto window = static_cast<GLFWwindow*>((Application::Get().GetWindow().GetNativeWindow()));
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>((Application::Get().GetWindow().GetNativeWindow()));
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	std::pair<int, int> Input::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>((Application::Get().GetWindow().GetNativeWindow()));
		double xPos,yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return std::pair<int, int>((int)xPos, (int)yPos);
	}
}

