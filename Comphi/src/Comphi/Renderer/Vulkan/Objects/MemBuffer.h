#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer
	{
	public:

		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
		VkBuffer bufferObj; //change to DATA
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;

		static uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		
		static void copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer);
		void copyBufferTo(MemBuffer& dstBuffer);
	
		virtual void cleanUp();

	protected :
		MemBuffer() = default;
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void InitMemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
	};

}

