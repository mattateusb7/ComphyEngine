#pragma once
#include "Comphi/Renderer/IUniformBuffer.h"
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer
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
	};



}

