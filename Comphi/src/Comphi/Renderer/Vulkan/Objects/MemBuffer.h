#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer // TODO: IBuffer to concatenate shared buffers into single packaged buffer
	{
	public:

		MemBuffer() = default;
		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static void copyBufferTo(VkBuffer& srcBuffer, VkBuffer& dstBuffer, uint copySize);

		void allocateMemoryBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;

		void cleanUp();

		//TODO: Maybe add static syncObj for memoryCopy Transfer Command Operations ?

	};

}

