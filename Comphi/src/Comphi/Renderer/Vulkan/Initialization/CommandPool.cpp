#include "cphipch.h"
#include "CommandPool.h"

namespace Comphi::Vulkan {

    SyncObjectsFactory CommandPool::commandBuffersSyncObjects = SyncObjectsFactory();
    VkFence CommandPool::commandBuffersFence;

	CommandPool::CommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;

		vkCheckError(vkCreateCommandPool(GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &graphicsCommandPool)) {
			COMPHILOG_CORE_FATAL("failed to create command pool!");
			throw std::runtime_error("failed to create command pool!");
			return;
		}

		VkCommandPoolCreateInfo poolInfoTransfer{};
		poolInfoTransfer.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfoTransfer.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
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

        //TODO: reuse same Command Buffer, within a queue, between multiple operations to reduce alocation overhead
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

        if (commandBuffersFence == nullptr) {
            commandBuffersSyncObjects.createFences(&commandBuffersFence, 1);
            vkResetFences(GraphicsHandler::get()->logicalDevice, 1, &commandBuffersFence);
        }

        vkQueueSubmit(commandQueue, 1, &submitInfo, commandBuffersFence);
        vkWaitForFences(GraphicsHandler::get()->logicalDevice, 1, &commandBuffersFence, VK_TRUE, UINT64_MAX);
        //vkQueueWaitIdle(commandQueue);

        /*
        We could use a fence and wait with vkWaitForFences,
        or simply wait for the transfer queue to become idle with vkQueueWaitIdle.
        A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete, instead of executing one at a time.
        That may give the driver more opportunities to optimize.
        */

        vkFreeCommandBuffers(GraphicsHandler::get()->logicalDevice, commandPool, 1, &commandBuffer.buffer);
        vkResetFences(GraphicsHandler::get()->logicalDevice, 1, &commandBuffersFence);

    }

    VkCommandPool CommandPool::getCommandPool(CommandQueueOperation& op) {
        switch (op)
        {
        case TransferCommand:
            return GraphicsHandler::get()->graphicsQueueFamily.commandPool;
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
