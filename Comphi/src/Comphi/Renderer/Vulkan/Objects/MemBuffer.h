#pragma once
#include "Comphi/Renderer/IObjects/IVertexBuffer.h"
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace Comphi::Vulkan {

	class MemBuffer
	{
	public:

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

		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const GraphicsHandler& graphicsHandler);

		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;
	
		GraphicsHandler graphicsHandler;

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		~MemBuffer();

		static void copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer);
		void copyBufferTo(MemBuffer& dstBuffer);
	};

}

