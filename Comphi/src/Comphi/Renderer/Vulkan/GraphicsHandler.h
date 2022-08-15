#pragma once
#include "GraphicsContext.h"

namespace Comphi::Vulkan {

	struct GraphicsHandler {
		std::shared_ptr<VkDevice> logicalDevice;
		std::shared_ptr<VkPhysicalDevice> physicalDevice;
		std::shared_ptr<VkCommandPool> transferCommandPool;
		std::shared_ptr<VkQueue> transferQueue;

		GraphicsHandler(
			const VkDevice& logicalDevice,
			const VkPhysicalDevice& physicalDevice,
			const VkCommandPool& transferCommandPool,
			const VkQueue& transferQueue)
		{
			this->logicalDevice = std::make_shared<VkDevice>(logicalDevice);
			this->physicalDevice = std::make_shared<VkPhysicalDevice>(physicalDevice);
			this->transferCommandPool = std::make_shared<VkCommandPool>(transferCommandPool);
			this->transferQueue = std::make_shared<VkQueue>(transferQueue);
		}
	};
}
