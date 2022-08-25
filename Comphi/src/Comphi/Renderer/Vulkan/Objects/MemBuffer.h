#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer
	{
	public:
		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;
		std::shared_ptr<GraphicsHandler> graphicsHandler;

		static uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		~MemBuffer();
		static void copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer);
		static VkCommandBuffer beginCommandBuffer(const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		static void endCommandBuffer(VkCommandBuffer& commandBuffer, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
		void copyBufferTo(MemBuffer& dstBuffer);
	
	protected :
		MemBuffer() = default;
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void InitMemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
	};

}

