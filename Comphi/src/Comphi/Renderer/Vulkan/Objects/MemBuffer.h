#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer
	{
	public:

		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, GraphicsHandler& graphicsHandler);

		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;
	
		std::shared_ptr<GraphicsHandler> graphicsHandler; //WTF!

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		~MemBuffer();

		static void copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer);
		void copyBufferTo(MemBuffer& dstBuffer);
	};

}

