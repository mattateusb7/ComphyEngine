#pragma once
#include "MemBuffer.h"

namespace Comphi::Vulkan {

    template<typename T>
    class UniformBuffer : public MemBuffer
    {
    public:
        UniformBuffer(const T* dataArray, const uint count, BufferUsage usage = BufferUsage::UniformBuffer);
    };

    template<typename T>
    UniformBuffer<T>::UniformBuffer(const T* dataArray, const uint count, BufferUsage usage)
    {
        bufferSize = sizeof(dataArray[0]) * count;

        VkBufferUsageFlagBits usageFlags;
        switch (usage)
        {
        case BufferUsage::UniformBuffer:
            usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            break;
        case BufferUsage::VertexBuffer:
            usageFlags = VkBufferUsageFlagBits(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            break;
        case BufferUsage::IndexBuffer:
            usageFlags = VkBufferUsageFlagBits(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            break;
        default:
            break;
        }

        if (usage == BufferUsage::UniformBuffer) {
            allocateMemoryBuffer(bufferSize,
                usageFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            return;
        }

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.updateBufferData(dataArray);

        allocateMemoryBuffer(bufferSize,
            usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        MemBuffer::copyBufferTo(stagingBuffer.bufferObj, bufferObj, (size_t)bufferSize);

        //cleanup
        stagingBuffer.cleanUp();
    }
}