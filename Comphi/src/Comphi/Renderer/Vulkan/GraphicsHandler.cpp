#include "cphipch.h"
#include "GraphicsHandler.h"

namespace Comphi::Vulkan {

	static std::shared_ptr<GraphicsHandler> graphicsHandler = std::make_shared<GraphicsHandler>();
	//static GraphicsHandler graphicsHandler = GraphicsHandler();

	GraphicsHandler* GraphicsHandler::get()
	{
		return graphicsHandler.get();
	}

	void GraphicsHandler::DeleteStatic()
	{
		this->isInUse = false;
		this->~GraphicsHandler();
	}

	GraphicsHandler::~GraphicsHandler()
	{
		//KEEP Static Reference after program closed
		if(isInUse) 
			graphicsHandler = std::make_shared<GraphicsHandler>(*graphicsHandler.get());
	}

    CommandBuffer GraphicsHandler::beginCommandBuffer(CommandQueueOperation op)
    {
        VkCommandPool commandPool = getCommandPool(op);

        CommandBuffer commandBuffer = { op };

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1; //how many command buffers to create

        vkAllocateCommandBuffers(*GraphicsHandler::get()->logicalDevice.get(), &allocInfo, &commandBuffer.buffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer.buffer, &beginInfo);

        return commandBuffer;

    }

    void GraphicsHandler::endCommandBuffer(CommandBuffer& commandBuffer)
    {
        VkQueue commandQueue = getCommandQueue(commandBuffer.op);
        VkCommandPool commandPool = getCommandPool(commandBuffer.op);

        vkEndCommandBuffer(commandBuffer.buffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.buffer;

        vkQueueSubmit(commandQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(commandQueue);

        /*
        We could use a fence and wait with vkWaitForFences,
        or simply wait for the transfer queue to become idle with vkQueueWaitIdle.
        A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete, instead of executing one at a time.
        That may give the driver more opportunities to optimize.
        */

        vkFreeCommandBuffers(*GraphicsHandler::get()->logicalDevice.get(), commandPool, 1, &commandBuffer.buffer);
    }

#pragma region Protected

    VkCommandPool GraphicsHandler::getCommandPool(CommandQueueOperation& op) {
        switch (op)
        {
        case TransferCommand:
            return *GraphicsHandler::get()->transferQueueFamily.commandPool.get();
            break;
        case GraphicsCommand:
        default:
            return *GraphicsHandler::get()->graphicsQueueFamily.commandPool.get();
            break;
        }

    }

    VkQueue GraphicsHandler::getCommandQueue(CommandQueueOperation& op) {
        switch (op)
        {
        case TransferCommand:
            return *GraphicsHandler::get()->transferQueueFamily.queue.get();
            break;
        case GraphicsCommand:
        default:
            return *GraphicsHandler::get()->graphicsQueueFamily.queue.get();
            break;
        }
    }
#pragma endregion

}
