#include "cphipch.h"
#include "GraphicsHandler.h"

namespace Comphi::Vulkan {

	static Comphi::Vulkan::GraphicsHandler graphicsHandler = GraphicsHandler();
	
	GraphicsHandler::GraphicsHandler() :
		WindowHandler(NULL, NULL),
		DeviceHandler(NULL, NULL),
		QueueHandler(NULL, NULL, NULL, NULL, NULL, NULL) {

	}

	void GraphicsHandler::setGraphicsHandler(
		GLFWwindow* windowHandle,
		const VkSurfaceKHR& surface,
		const VkDevice& logicalDevice,
		const VkPhysicalDevice& physicalDevice,
		const uint32_t transferQueueFamilyIndex,
		const VkCommandPool& transferCommandPool,
		const VkQueue& transferQueue,
		const uint32_t graphicsQueueFamilyIndex,
		const VkCommandPool& graphicsCommandPool,
		const VkQueue& graphicsQueue
	)
	{
		graphicsHandler = GraphicsHandler(windowHandle, surface,
			logicalDevice, physicalDevice,
			transferQueueFamilyIndex, transferCommandPool, transferQueue,
			graphicsQueueFamilyIndex, graphicsCommandPool, graphicsQueue) ;
	}

	GraphicsHandler* GraphicsHandler::get()
	{
		return &graphicsHandler;
	}
}
