#include "cphipch.h"
#include "IndexBuffer.h"

namespace Comphi::Vulkan {

    IndexBuffer::IndexBuffer(const IndexArray& indices, GraphicsHandler& graphicsHandler)
    {
        indexCount = static_cast<uint16_t>(indices.size());
        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            graphicsHandler);

        void* data; //copy data to staging buffer
        vkMapMemory(*graphicsHandler.logicalDevice.get(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(*graphicsHandler.logicalDevice.get(), stagingBuffer.bufferMemory);

        buffer = std::make_unique<MemBuffer>(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            graphicsHandler);

        stagingBuffer.copyBufferTo(*buffer.get());
    }

    void IndexBuffer::bind()
    {
        
    }

    void IndexBuffer::unbind()
    {
        
    }

}