#include "cphipch.h"
#include "MemBuffer.h"
#include "Comphi/Renderer/Vulkan/Commands/CommandPool.h"

namespace Comphi::Vulkan {
    
    MemBuffer::MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        allocateMemoryBuffer(size, usage, properties);
    }

    void MemBuffer::allocateMemoryBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        bufferSize = size;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vkCheckError(vkCreateBuffer(GraphicsHandler::get()->logicalDevice, &bufferInfo, nullptr, &bufferObj)) {
            COMPHILOG_CORE_ERROR("failed to create buffer!");
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(GraphicsHandler::get()->logicalDevice, bufferObj, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = MemBuffer::findMemoryType(memRequirements.memoryTypeBits, properties);

        vkCheckError(vkAllocateMemory(GraphicsHandler::get()->logicalDevice, &allocInfo, nullptr, &bufferMemory)) {
            COMPHILOG_CORE_ERROR("failed to allocate vertex buffer memory!");
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(GraphicsHandler::get()->logicalDevice, bufferObj, bufferMemory, 0);
    }

    uint32_t MemBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(GraphicsHandler::get()->physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        COMPHILOG_CORE_ERROR("failed to find suitable memory type!");
        throw std::runtime_error("failed to find suitable memory type!");

    }


    void MemBuffer::copyBufferTo(VkBuffer& srcBuffer, VkBuffer& dstBuffer, uint copySize)
    {
        CommandBuffer commandBuffer = CommandPool::beginCommandBuffer(TransferCommand);

        VkBufferCopy copyRegion{};
        //copyRegion.srcOffset = 0; // Optional
        //copyRegion.dstOffset = 0; // Optional
        copyRegion.size = copySize;
        vkCmdCopyBuffer(commandBuffer.buffer, srcBuffer, dstBuffer, 1, &copyRegion);

        CommandPool::endCommandBuffer(commandBuffer);
    }
    
    void MemBuffer::cleanUp()
    {
        COMPHILOG_CORE_INFO("vkDestroy Destroy MemBuffer");
        vkDestroyBuffer(GraphicsHandler::get()->logicalDevice, bufferObj, nullptr);
        vkFreeMemory(GraphicsHandler::get()->logicalDevice, bufferMemory, nullptr);
    }

}