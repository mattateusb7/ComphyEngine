#include "cphipch.h"
#include "IndexBuffer.h"

namespace Comphi::Vulkan {

    IndexBuffer::IndexBuffer(const IndexArray& indices)
    {
        i_indexCount = static_cast<uint16_t>(indices.size());
        bufferSize = sizeof(indices[0]) * i_indexCount;

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data; //copy data to staging buffer
        vkMapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory);

        allocateMemoryBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        GraphicsHandler::copyBufferTo(stagingBuffer.bufferObj,bufferObj, (size_t)bufferSize);

        //cleanup
        vkDestroyBuffer(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferObj, nullptr);
        vkFreeMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory, nullptr);
    }

}