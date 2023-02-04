#pragma once
#include "../GraphicsHandler.h"
#include "SyncObjectsFactory.h"
#include "Comphi/Renderer/IGraphicsPipeline.h"

namespace Comphi::Vulkan {

	enum CommandQueueOperation { TransferCommand, GraphicsCommand };

	struct CommandBuffer {
		CommandQueueOperation op;
		VkCommandBuffer buffer;
		VkFence* fence;
		VkSemaphore* signalSemaphore;
		VkSemaphore* waitSemaphore;
		VkPipelineStageFlags* waitDstStageMask;
	};

	//Allocator for CommandBuffers
	class CommandPool
	{
	public:
		CommandPool(); 

		static CommandBuffer beginCommandBuffer(CommandQueueOperation op);
		static void endCommandBuffer(CommandBuffer& commandBuffer);

		void allocateGraphicsCommandBuffer(VkCommandBuffer* commandBuffers, uint count);
		void allocateTransferCommandBuffer(VkCommandBuffer* commandBuffers, uint count);

		VkCommandPool graphicsCommandPool;
		VkCommandPool transferCommandPool;

		void cleanUp(); //When a pool is destroyed, all command buffers allocated from the pool are freed.
		
	protected:
		static VkCommandPool getCommandPool(CommandQueueOperation& op);
		static VkQueue getCommandQueue(CommandQueueOperation& op);
	};

}

