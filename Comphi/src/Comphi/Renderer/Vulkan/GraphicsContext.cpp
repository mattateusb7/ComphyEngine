#include "cphipch.h"
#include "GraphicsContext.h"

#include "Comphi/Renderer/GraphicsAPI.h"
#include "Comphi/Platform/Windows/FileRef.h"

namespace Comphi::Vulkan {

	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		COMPHILOG_CORE_ASSERT(m_WindowHandle, "Window Handle is NULL!");
		
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		COMPHILOG_CORE_INFO("Vk_extensions supported : {0}", extensionCount);
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		/***DEBUG***/

		/***DEBUG***/
	}

	void GraphicsContext::Draw()
	{
		
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}