#include "cphipch.h"
#include "UniformBuffer.h"

namespace Comphi::Vulkan {
    
    UniformBuffer::UniformBuffer(const void* dataArray, const uint size, const uint count, BufferUsage usage)
    {
        bufferSize = size * count;
        bufferUsage = usage;

        VkBufferUsageFlagBits usageFlags;
        VkMemoryPropertyFlags accessFlags;
        switch (bufferUsage)
        {
        case BufferUsage::UniformBuffer:
            usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            accessFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            break;
        case BufferUsage::VertexBuffer:
            usageFlags = VkBufferUsageFlagBits(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            accessFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        case BufferUsage::IndexBuffer:
            usageFlags = VkBufferUsageFlagBits(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            accessFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        default:
            break;
        }

        if (bufferUsage == BufferUsage::UniformBuffer) {
            allocateMemoryBuffer(bufferSize,
                usageFlags, accessFlags);
            return;
        }

        allocateMemoryBuffer(bufferSize,
            usageFlags, accessFlags);

        updateBufferData(dataArray);
    }

    void Comphi::Vulkan::UniformBuffer::updateBufferData(const void* dataArray)
    {
        if (bufferUsage == BufferUsage::UniformBuffer) {
            copyData(*this, dataArray);
            return;
        }

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        copyData(stagingBuffer, dataArray);

        MemBuffer::copyBufferTo(stagingBuffer.bufferObj, bufferObj, (size_t)bufferSize);

        //cleanup
        stagingBuffer.cleanUp();
    }

    void UniformBuffer::copyData(const MemBuffer& membuffer, const void* dataArray)
    {
        void* deviceMemoryData;
        vkMapMemory(GraphicsHandler::get()->logicalDevice, membuffer.bufferMemory, 0, membuffer.bufferSize, 0, &deviceMemoryData);
        memcpy(deviceMemoryData, dataArray, (size_t)bufferSize);
        vkUnmapMemory(GraphicsHandler::get()->logicalDevice, membuffer.bufferMemory);
    }
}