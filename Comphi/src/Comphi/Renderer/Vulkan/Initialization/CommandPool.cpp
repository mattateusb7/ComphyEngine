#include "cphipch.h"
#include "CommandPool.h"

namespace Comphi::Vulkan {

	CommandPool::CommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;

		vkCheckError(vkCreateCommandPool(GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &graphicsCommandPool)) {
			COMPHILOG_CORE_FATAL("failed to create command pool!");
			throw std::runtime_error("failed to create command pool!");
			return;
		}

		VkCommandPoolCreateInfo poolInfoTransfer{};
		poolInfoTransfer.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfoTransfer.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
		poolInfoTransfer.queueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;

		vkCheckError(vkCreateCommandPool(GraphicsHandler::get()->logicalDevice, &poolInfoTransfer, nullptr, &transferCommandPool)) {
			COMPHILOG_CORE_FATAL("failed to create transfer command pool!");
			throw std::runtime_error("failed to create transfer command pool!");
			return;
		}

		GraphicsHandler::get()->setCommandPools(transferCommandPool, graphicsCommandPool);
	}


	void CommandPool::allocateGraphicsCommandBuffer(VkCommandBuffer* commandBuffers, uint count) {

		VkCommandBufferAllocateInfo allocInfo_graphics{};
		allocInfo_graphics.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo_graphics.commandPool = graphicsCommandPool;
		allocInfo_graphics.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo_graphics.commandBufferCount = static_cast<uint32_t>(count);

		vkCheckError(vkAllocateCommandBuffers(GraphicsHandler::get()->logicalDevice, &allocInfo_graphics, &commandBuffers[0])) {
			COMPHILOG_CORE_FATAL("failed to allocate graphics command buffers!");
			throw std::runtime_error("failed to allocate graphics command buffers!");
			return;
		}

		COMPHILOG_CORE_INFO("Allocated {0} GraphicsCommandBuffers from graphicsCommandPool", count);

	}

	void CommandPool::allocateTransferCommandBuffer(VkCommandBuffer* commandBuffers, uint count) {
		
		VkCommandBufferAllocateInfo allocInfo_transfer{};
		allocInfo_transfer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo_transfer.commandPool = transferCommandPool;
		allocInfo_transfer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo_transfer.commandBufferCount = static_cast<uint32_t>(count);


		vkCheckError(vkAllocateCommandBuffers(GraphicsHandler::get()->logicalDevice, &allocInfo_transfer, &commandBuffers[0])) {
			COMPHILOG_CORE_FATAL("failed to allocate transfer command buffers!");
			throw std::runtime_error("failed to allocate transfer command buffers!");
			return;
		}

		COMPHILOG_CORE_INFO("Allocated {0} TransferCommandBuffers from transferCommandPool", count);

	}

    CommandBuffer CommandPool::beginCommandBuffer(CommandQueueOperation op)
    {
        VkCommandPool commandPool = getCommandPool(op);

        CommandBuffer commandBuffer = { op };

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1; //how many command buffers to create

        vkAllocateCommandBuffers(GraphicsHandler::get()->logicalDevice, &allocInfo, &commandBuffer.buffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer.buffer, &beginInfo);

        return commandBuffer;

    }

    void CommandPool::endCommandBuffer(CommandBuffer& commandBuffer)
    {
        VkQueue commandQueue = getCommandQueue(commandBuffer.op);
        VkCommandPool commandPool = getCommandPool(commandBuffer.op);

        vkEndCommandBuffer(commandBuffer.buffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.buffer;

        if (commandBuffer.signalSemaphore != VK_NULL_HANDLE) {
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = commandBuffer.signalSemaphore;
        } 
        
        if (commandBuffer.waitSemaphore != VK_NULL_HANDLE) {
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = commandBuffer.waitSemaphore;
            submitInfo.pWaitDstStageMask = commandBuffer.waitDstStageMask;
        }

        if (commandBuffer.fence != VK_NULL_HANDLE) {
            vkQueueSubmit(commandQueue, 1, &submitInfo, *commandBuffer.fence);
            vkWaitForFences(GraphicsHandler::get()->logicalDevice, 1, commandBuffer.fence, VK_TRUE, UINT64_MAX);
            vkResetFences(GraphicsHandler::get()->logicalDevice, 1, commandBuffer.fence);
            vkFreeCommandBuffers(GraphicsHandler::get()->logicalDevice, commandPool, 1, &commandBuffer.buffer);
            //vkFreeCommandBuffers is typically called once per frame, not immediately after the command buffer is submitted. 
            //This is because you typically want to keep the command buffer around for a little while in case you need to resubmit it.
        }
        else {
            vkQueueSubmit(commandQueue, 1, &submitInfo, 0);
            vkQueueWaitIdle(commandQueue);
            vkFreeCommandBuffers(GraphicsHandler::get()->logicalDevice, commandPool, 1, &commandBuffer.buffer);
        }

        
    }

    VkCommandPool CommandPool::getCommandPool(CommandQueueOperation& op) {
        switch (op)
        {
        case TransferCommand:
            return GraphicsHandler::get()->transferQueueFamily.commandPool;
            break;
        case GraphicsCommand:
        default:
            return GraphicsHandler::get()->graphicsQueueFamily.commandPool;
            break;
        }

    }

    VkQueue CommandPool::getCommandQueue(CommandQueueOperation& op) {
        switch (op)
        {
        case TransferCommand:
            return GraphicsHandler::get()->transferQueueFamily.queue;
            break;
        case GraphicsCommand:
        default:
            return GraphicsHandler::get()->graphicsQueueFamily.queue;
            break;
        }
    }

	void CommandPool::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy transferCommandPool");
		vkDestroyCommandPool(GraphicsHandler::get()->logicalDevice, transferCommandPool, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy graphicsCommandPool");
		vkDestroyCommandPool(GraphicsHandler::get()->logicalDevice, graphicsCommandPool, nullptr);
	}

}
