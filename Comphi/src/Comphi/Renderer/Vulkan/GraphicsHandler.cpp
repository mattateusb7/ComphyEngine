#include "cphipch.h"
#include "GraphicsHandler.h"
#include "Initialization/SyncObjectsFactory.h"

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

        vkAllocateCommandBuffers(GraphicsHandler::get()->logicalDevice, &allocInfo, &commandBuffer.buffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer.buffer, &beginInfo);

        return commandBuffer;

    }
    VkFence commandBufferFence;
    void GraphicsHandler::endCommandBuffer(CommandBuffer& commandBuffer)
    {
        VkQueue commandQueue = getCommandQueue(commandBuffer.op);
        VkCommandPool commandPool = getCommandPool(commandBuffer.op);

        vkEndCommandBuffer(commandBuffer.buffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.buffer;

        
        if (commandBufferFence == nullptr) {
            SyncObjectsFactory::createFences(&commandBufferFence, 1);
            vkResetFences(GraphicsHandler::get()->logicalDevice, 1, &commandBufferFence);
        }

        vkQueueSubmit(commandQueue, 1, &submitInfo, commandBufferFence);
        vkWaitForFences(GraphicsHandler::get()->logicalDevice, 1, &commandBufferFence, VK_TRUE, UINT64_MAX);
        //vkQueueWaitIdle(commandQueue);

        /*
        We could use a fence and wait with vkWaitForFences,
        or simply wait for the transfer queue to become idle with vkQueueWaitIdle.
        A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete, instead of executing one at a time.
        That may give the driver more opportunities to optimize.
        */

        vkFreeCommandBuffers(GraphicsHandler::get()->logicalDevice, commandPool, 1, &commandBuffer.buffer);
        vkResetFences(GraphicsHandler::get()->logicalDevice, 1, &commandBufferFence);

    }

    uint32_t GraphicsHandler::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(get()->physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        COMPHILOG_CORE_ERROR("failed to find suitable memory type!");
        throw std::runtime_error("failed to find suitable memory type!");

    }

    void GraphicsHandler::copyBufferTo(VkBuffer& srcBuffer, VkBuffer& dstBuffer, uint copySize)
    {
        CommandBuffer commandBuffer = GraphicsHandler::beginCommandBuffer(TransferCommand);

        VkBufferCopy copyRegion{};
        //copyRegion.srcOffset = 0; // Optional
        //copyRegion.dstOffset = 0; // Optional
        copyRegion.size = copySize;
        vkCmdCopyBuffer(commandBuffer.buffer, srcBuffer, dstBuffer, 1, &copyRegion);

        GraphicsHandler::endCommandBuffer(commandBuffer);
    }

#pragma region Protected

    VkCommandPool GraphicsHandler::getCommandPool(CommandQueueOperation& op) {
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

    VkQueue GraphicsHandler::getCommandQueue(CommandQueueOperation& op) {
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
#pragma endregion

}
