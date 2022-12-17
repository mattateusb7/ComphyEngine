#include "cphipch.h"
#include "MemBuffer.h"

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
        allocInfo.memoryTypeIndex = GraphicsHandler::findMemoryType(memRequirements.memoryTypeBits, properties);

        vkCheckError(vkAllocateMemory(GraphicsHandler::get()->logicalDevice, &allocInfo, nullptr, &bufferMemory)) {
            COMPHILOG_CORE_ERROR("failed to allocate vertex buffer memory!");
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(GraphicsHandler::get()->logicalDevice, bufferObj, bufferMemory, 0);
    }

    void MemBuffer::cleanUp()
    {
        COMPHILOG_CORE_INFO("vkDestroy Destroy MemBuffer");
        vkDestroyBuffer(GraphicsHandler::get()->logicalDevice, bufferObj, nullptr);
        vkFreeMemory(GraphicsHandler::get()->logicalDevice, bufferMemory, nullptr);
    }
}