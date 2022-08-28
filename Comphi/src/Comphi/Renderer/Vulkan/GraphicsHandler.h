#pragma once
#include "Common.h"

namespace Comphi::Vulkan {

	struct CommandQueueFamily {
		uint32_t index;
		std::shared_ptr<VkCommandPool> commandPool;
		std::shared_ptr<VkQueue> queue;
	};

	struct GraphicsHandler {
		std::shared_ptr<VkDevice> logicalDevice;
		std::shared_ptr<VkPhysicalDevice> physicalDevice;
		CommandQueueFamily transferQueueFamily;
		CommandQueueFamily graphicsQueueFamily;
		GraphicsHandler(
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
			this->logicalDevice = std::make_shared<VkDevice>(logicalDevice);
			this->physicalDevice = std::make_shared<VkPhysicalDevice>(physicalDevice);

			this->transferQueueFamily.index = transferQueueFamilyIndex;
			this->transferQueueFamily.commandPool = std::make_shared<VkCommandPool>(transferCommandPool);
			this->transferQueueFamily.queue = std::make_shared<VkQueue>(transferQueue);

			this->graphicsQueueFamily.index = graphicsQueueFamilyIndex;
			this->graphicsQueueFamily.commandPool = std::make_shared<VkCommandPool>(transferCommandPool);
			this->graphicsQueueFamily.queue = std::make_shared<VkQueue>(transferQueue);
		}
	};
}
