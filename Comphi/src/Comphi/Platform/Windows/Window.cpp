#include "cphipch.h"
#include "Window.h"
#include "Comphi/API/ComphiAPI.h"

Comphi::IWindow* Comphi::IWindow::Create(const WindowProperties& props)
{
	return new Comphi::Windows::Window(props);
}

namespace Comphi::Windows {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		COMPHILOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProperties& props)
	{
		Init(props);
	}	
	
	void Window::Init(const WindowProperties& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			COMPHILOG_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		COMPHILOG_CORE_INFO("GLFW Initialized.");

		//Select ComphiAPI
		ComphiAPI::select::Vulkan();

		switch (ComphiAPI::getActiveAPI()) {
			case ComphiAPI::Vulkan: {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
				//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //block window resize (make into property?)
				break;
			}
		};

		m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(props.VSync);

		//Set GLFW Callbacks
		{
			//WINDOW FRAMEBUFFER SIZE CALLBACK
			glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {

				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				FramebufferResizedEvent event(width, height);
				data.EventCallback(event);

			});

			//WINDOW SIZE CALLBACK
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizedEvent event(width, height);
				data.EventCallback(event);
			});

			//WINDOW CLOSE CALLBACK
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

			//KEY CALLBACK
			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

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

			//KEY TYPED CALLBACK
			glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint keycode)
			{
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

			//MOUSE BTN CALLBACK
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

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
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event(x, y);
				data.EventCallback(event);
			});

			//MOUSE POS CALLBACK
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
			{
				WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((uint)x, (uint)y);
				data.EventCallback(event);
			});
		}

		COMPHILOG_CORE_INFO("Creating Window {0} ({1},{2}) - Success!", props.Title, props.Width, props.Height);

		m_GraphicsContext.reset(ComphiAPI::Init::GraphicsContext(m_Window));
		m_GraphicsContext->Init();
	}

	void Window::SetEventCallback(const EventCallback& callback)
	{
		m_Data.EventCallback = callback;
	}

	void Window::Shutdown()
	{
		COMPHILOG_CORE_WARN("Window Shutdown !");
		m_GraphicsContext->CleanUp();
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
	}

	void Window::OnBeginUpdate(SceneGraphPtr& sceneGraph)
	{
		m_GraphicsContext->SetScenes(sceneGraph);
		m_GraphicsContext->Draw();
	}

	void Window::OnWindowResized(uint x, uint y)
	{
		m_GraphicsContext->ResizeWindow(x, y);
	}

	void Window::OnFramebufferResized(uint x, uint y)
	{
		m_GraphicsContext->ResizeFramebuffer(x, y);
	}

	void Window::SetVSync(bool enabled)
	{
		switch (ComphiAPI::getActiveAPI()) {
			case ComphiAPI::RenderingAPI::Vulkan: {
				// TODO: togle VSYNC
				break;
			}
		}

		m_Data.VSync = enabled;
	}





}


