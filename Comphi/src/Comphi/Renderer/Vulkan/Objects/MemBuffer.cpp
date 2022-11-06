#include "cphipch.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {
    
    MemBuffer::MemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        InitMemBuffer(size, usage, properties);
    }

    void MemBuffer::InitMemBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        bufferSize = size;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(*GraphicsHandler::get()->logicalDevice.get(), &bufferInfo, nullptr, &bufferObj) != VK_SUCCESS) {
            COMPHILOG_CORE_ERROR("failed to create buffer!");
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(*GraphicsHandler::get()->logicalDevice.get(), bufferObj, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(*GraphicsHandler::get()->logicalDevice.get(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            COMPHILOG_CORE_ERROR("failed to allocate vertex buffer memory!");
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(*GraphicsHandler::get()->logicalDevice.get(), bufferObj, bufferMemory, 0);
    }

    uint32_t MemBuffer::findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        COMPHILOG_CORE_ERROR("failed to find suitable memory type!");
        throw std::runtime_error("failed to find suitable memory type!");

    }

    uint32_t MemBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        return findMemoryType(*GraphicsHandler::get()->physicalDevice.get(),typeFilter,properties);
    }

    void MemBuffer::cleanUp()
    {
        COMPHILOG_CORE_INFO("vkDestroy Destroy MemBuffer");
        vkDestroyBuffer(*GraphicsHandler::get()->logicalDevice.get(), bufferObj, nullptr);
        vkFreeMemory(*GraphicsHandler::get()->logicalDevice.get(),bufferMemory, nullptr);
    }

    void MemBuffer::copyBuffer(MemBuffer& srcBuffer, MemBuffer& dstBuffer)
    {
        srcBuffer.copyBufferTo(dstBuffer);
    }


    void MemBuffer::copyBufferTo(MemBuffer& dstBuffer)
    {
        CommandBuffer commandBuffer = GraphicsHandler::beginCommandBuffer(TransferCommand);

        VkBufferCopy copyRegion{};
        //copyRegion.srcOffset = 0; // Optional
        //copyRegion.dstOffset = 0; // Optional
        copyRegion.size = bufferSize;
        vkCmdCopyBuffer(commandBuffer.buffer, bufferObj, dstBuffer.bufferObj, 1, &copyRegion);

        GraphicsHandler::endCommandBuffer(commandBuffer);

    }
}