#include "cphipch.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {


    MemBuffer::MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, GraphicsHandler& graphicsHandler)
    {
        this->graphicsHandler = std::make_shared<GraphicsHandler>(graphicsHandler);

        bufferSize = size;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(*this->graphicsHandler->logicalDevice.get(), &bufferInfo, nullptr, &bufferObj) != VK_SUCCESS) {
            COMPHILOG_CORE_ERROR("failed to create buffer!");
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(*this->graphicsHandler->logicalDevice.get(), bufferObj, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(*this->graphicsHandler->logicalDevice.get(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            COMPHILOG_CORE_ERROR("failed to allocate vertex buffer memory!");
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(*this->graphicsHandler->logicalDevice.get(), bufferObj, bufferMemory, 0);
    }

    uint32_t MemBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(*this->graphicsHandler->physicalDevice.get(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        COMPHILOG_CORE_ERROR("failed to find suitable memory type!");
        throw std::runtime_error("failed to find suitable memory type!");

    }

    MemBuffer::~MemBuffer()
    {
        COMPHILOG_CORE_INFO("vkDestroy Destroy Buffer");
        vkDestroyBuffer(*graphicsHandler->logicalDevice.get(), bufferObj, nullptr);
        vkFreeMemory(*graphicsHandler->logicalDevice.get(),bufferMemory, nullptr);
    }

    void MemBuffer::copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer)
    {
        srcBuffer.copyBufferTo(dstBuffer);
    }

    void MemBuffer::copyBufferTo(MemBuffer& dstBuffer)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = *graphicsHandler->transferCommandPool.get();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(*graphicsHandler->logicalDevice.get(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = bufferSize;
        vkCmdCopyBuffer(commandBuffer, bufferObj, dstBuffer.bufferObj, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(*graphicsHandler->transferQueue.get(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(*graphicsHandler->transferQueue);

        /*
        We could use a fence and wait with vkWaitForFences,
        or simply wait for the transfer queue to become idle with vkQueueWaitIdle.
        A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete, instead of executing one at a time.
        That may give the driver more opportunities to optimize.
        */

        vkFreeCommandBuffers(*graphicsHandler->logicalDevice.get(), *graphicsHandler->transferCommandPool.get(), 1, &commandBuffer);

    }


}