#include "cphipch.h"
#include "GraphicsHandler.h"

#include "Initialization/CommandPool.h"

namespace Comphi::Vulkan {

	static std::shared_ptr<GraphicsHandler> graphicsHandler = std::make_shared<GraphicsHandler>();

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
        CommandBuffer commandBuffer = CommandPool::beginCommandBuffer(TransferCommand);

        VkBufferCopy copyRegion{};
        //copyRegion.srcOffset = 0; // Optional
        //copyRegion.dstOffset = 0; // Optional
        copyRegion.size = copySize;
        vkCmdCopyBuffer(commandBuffer.buffer, srcBuffer, dstBuffer, 1, &copyRegion);

        CommandPool::endCommandBuffer(commandBuffer);
    }

}
