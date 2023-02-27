#pragma once
#include "Comphi/Renderer/IUniformBuffer.h"
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer : public IUniformBuffer
	{
	public:

		MemBuffer() = default;
		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static void copyBufferTo(VkBuffer& srcBuffer, VkBuffer& dstBuffer, uint copySize);

		void allocateMemoryBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
		virtual void updateBufferData(const void* dataArray) override;

		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;

		void cleanUp() override;
	};



}

