#pragma once
#include "../GraphicsHandler.h"
#include "SyncObjectsFactory.h"

namespace Comphi::Vulkan {

	enum CommandQueueOperation { TransferCommand, GraphicsCommand };

	struct CommandBuffer {
		CommandQueueOperation op;
		VkCommandBuffer buffer;
	};

	//Allocator for CommandBuffers
	class CommandPool
	{
	public:
		CommandPool(); 

		static CommandBuffer beginCommandBuffer(CommandQueueOperation op);
		static void endCommandBuffer(CommandBuffer& commandBuffer);

		static VkCommandPool getCommandPool(CommandQueueOperation& op);
		static VkQueue getCommandQueue(CommandQueueOperation& op);

		void allocateGraphicsCommandBuffer(VkCommandBuffer* commandBuffers, uint count);
		void allocateTransferCommandBuffer(VkCommandBuffer* commandBuffers, uint count);

		VkCommandPool graphicsCommandPool;
		VkCommandPool transferCommandPool;

		static SyncObjectsFactory commandBuffersSyncObjects;
		static VkFence commandBuffersFence;

		void cleanUp(); //When a pool is destroyed, all command buffers allocated from the pool are freed.

	};

}

