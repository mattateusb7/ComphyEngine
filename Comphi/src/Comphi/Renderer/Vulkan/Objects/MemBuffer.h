#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer
	{
	public:
		enum CommandQueueOperation { MEM_TransferCommand, MEM_GraphicsCommand };

		struct CommandBuffer {
			CommandQueueOperation op = MEM_TransferCommand;
			VkCommandBuffer buffer;
		};

		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
		VkBuffer bufferObj; //change to DATA
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;

		static uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static CommandBuffer beginCommandBuffer(CommandQueueOperation op);
		static void endCommandBuffer(CommandBuffer& commandBuffer);
		
		static void copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer);
		void copyBufferTo(MemBuffer& dstBuffer);
	
		virtual void cleanUp();

	protected :
		MemBuffer() = default;
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void InitMemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
	};

}

