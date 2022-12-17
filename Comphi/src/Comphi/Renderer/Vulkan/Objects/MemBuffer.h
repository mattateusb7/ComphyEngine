#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer // TODO: IBuffer to concatenate shared buffers into single packaged buffer
	{
	public:

		MemBuffer() = default;
		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void allocateMemoryBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;

		void cleanUp();

	};

}

