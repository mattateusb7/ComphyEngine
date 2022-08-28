#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class MemBuffer
	{
	public:
		enum CommandQueueOperation { MEM_TransferCommand, MEM_GraphicsCommand };

		struct CommandBuffer {
			const std::shared_ptr<GraphicsHandler>& graphicsHandler;
			CommandQueueOperation op = MEM_TransferCommand;
			VkCommandBuffer buffer;
		};

		MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
		VkBuffer bufferObj;
		VkDeviceMemory bufferMemory;
		VkDeviceSize bufferSize;
		std::shared_ptr<GraphicsHandler> graphicsHandler;

		static uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		~MemBuffer();

		static CommandBuffer beginCommandBuffer(CommandQueueOperation op, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		static void endCommandBuffer(CommandBuffer& commandBuffer);
		
		static void copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer);
		void copyBufferTo(MemBuffer& dstBuffer);
	
	protected :
		MemBuffer() = default;
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void InitMemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
	};

}

