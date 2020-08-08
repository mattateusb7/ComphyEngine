#include "cphipch.h"
#include "WindowsWindow.h"

namespace Comphi {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		CPHI_LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProperties& props) 
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		Init(props);
	}	
	
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		CPHI_LOG_CORE_INFO("Creating Window {0} ({1},{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			CPHI_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		
		m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//Set GLFW Callbacks
		{
			//WINDOW SIZE CALLBACK
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizedEvent event(width, height);
				data.EventCallback(event);
			});

			//WINDOW CLOSE CALLBACK
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

			//KEY CALLBACK
			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

			//MOUSE BTN CALLBACK
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

			//MOUSE SCROLL CALLBACK
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event(x, y);
				data.EventCallback(event);
			});

			//MOUSE SCROLL CALLBACK
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event(x, y);
				data.EventCallback(event);
			});
		}
		
	}

	void WindowsWindow::SetEventCallback(const EventCallback& callback)
	{
		m_Data.EventCallback = callback;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window); 
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) 
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}





}


