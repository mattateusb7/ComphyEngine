#pragma once
#include "Common.h"

namespace Comphi::Vulkan {

	class DeviceHandler {
	public:
		std::shared_ptr<VkDevice> logicalDevice;
		std::shared_ptr<VkPhysicalDevice> physicalDevice;
		DeviceHandler(
			const VkDevice& logicalDevice,
			const VkPhysicalDevice& physicalDevice
		) {
			this->logicalDevice = std::make_shared<VkDevice>(logicalDevice);
			this->physicalDevice = std::make_shared<VkPhysicalDevice>(physicalDevice);
		}
	};

	class QueueHandler {
	public:
		struct CommandQueueFamily {
			uint32_t index;
			std::shared_ptr<VkCommandPool> commandPool;
			std::shared_ptr<VkQueue> queue;
		};
		CommandQueueFamily transferQueueFamily;
		CommandQueueFamily graphicsQueueFamily;
		QueueHandler(
			const uint32_t transferQueueFamilyIndex,
			const VkCommandPool& transferCommandPool,
			const VkQueue& transferQueue,
			const uint32_t graphicsQueueFamilyIndex,
			const VkCommandPool& graphicsCommandPool,
			const VkQueue& graphicsQueue
		) {
			this->transferQueueFamily.index = transferQueueFamilyIndex;
			this->transferQueueFamily.commandPool = std::make_shared<VkCommandPool>(transferCommandPool);
			this->transferQueueFamily.queue = std::make_shared<VkQueue>(transferQueue);

			this->graphicsQueueFamily.index = graphicsQueueFamilyIndex;
			this->graphicsQueueFamily.commandPool = std::make_shared<VkCommandPool>(graphicsCommandPool);
			this->graphicsQueueFamily.queue = std::make_shared<VkQueue>(graphicsQueue);
		}
	};

	class WindowHandler {
	public:
		std::shared_ptr<VkSurfaceKHR> surface;
		GLFWwindow* windowHandle;
		WindowHandler(
			GLFWwindow* windowHandle,
			const VkSurfaceKHR& surface
		) {
			this->surface = std::make_shared<VkSurfaceKHR>(surface);
			this->windowHandle = windowHandle;
		}

	};

	class GraphicsHandler : public DeviceHandler, public QueueHandler , public WindowHandler {
	protected :
		GraphicsHandler(
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
		) :
			WindowHandler(windowHandle, surface),
			DeviceHandler(logicalDevice, physicalDevice),
			QueueHandler(
				transferQueueFamilyIndex, transferCommandPool, transferQueue,
				graphicsQueueFamilyIndex, graphicsCommandPool, graphicsQueue
			)
		{}
	public:
		GraphicsHandler();
		static void setGraphicsHandler(
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
		);
		
		static GraphicsHandler* get();
	};
}
