#include "cphipch.h"
#include "VertexBuffer.h"

namespace Comphi::Vulkan {

    VertexBuffer::VertexBuffer(const VertexArray& vertices, const std::shared_ptr<GraphicsHandler>& graphicsHandler)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            graphicsHandler);

        void* data; //copy data to staging buffer
        vkMapMemory(*graphicsHandler->logicalDevice.get(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(*graphicsHandler->logicalDevice.get(), stagingBuffer.bufferMemory);

        InitMemBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            graphicsHandler);

        stagingBuffer.copyBufferTo(*(MemBuffer*)this);
    }

    void VertexBuffer::bind()
    {

    }

    void VertexBuffer::unbind()
    {
        
    }

}