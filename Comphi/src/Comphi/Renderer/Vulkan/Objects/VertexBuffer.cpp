#include "cphipch.h"
#include "VertexBuffer.h"

namespace Comphi::Vulkan {

    VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, const MemBuffer::GraphicsHandler& graphicsHandler)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            graphicsHandler);

        void* data; //copy data to staging buffer
        vkMapMemory(*graphicsHandler.logicalDevice.get(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(*graphicsHandler.logicalDevice.get(), stagingBuffer.bufferMemory);

        buffer = std::make_unique<MemBuffer>(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            graphicsHandler);

        stagingBuffer.copyBufferTo(*buffer.get());
    }

    void VertexBuffer::bind()
    {

    }

    void VertexBuffer::bind(VkCommandBuffer& commandBuffer)
    {
        VkBuffer vertexBuffers[] = { buffer->bufferObj };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }

    void VertexBuffer::unbind()
    {
        
    }

}