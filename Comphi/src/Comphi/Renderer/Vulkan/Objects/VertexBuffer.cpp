#include "cphipch.h"
#include "VertexBuffer.h"

namespace Comphi::Vulkan {

    VertexBuffer::VertexBuffer(const VertexArray& vertices)
    {
        i_vertexCount = static_cast<uint32_t>(vertices.size());
        bufferSize = sizeof(vertices[0]) * i_vertexCount;

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data; //copy data to staging buffer
        vkMapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory);

        allocateMemoryBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        MemBuffer::copyBufferTo(stagingBuffer.bufferObj, bufferObj, (size_t)bufferSize);

        //cleanup
        stagingBuffer.cleanUp();
    }

}