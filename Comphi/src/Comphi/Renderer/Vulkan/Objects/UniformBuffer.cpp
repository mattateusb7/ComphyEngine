#include "cphipch.h"
#include "UniformBuffer.h"

namespace Comphi::Vulkan {

    UniformBuffer::UniformBuffer(const UniformBufferObject& ubo, const std::shared_ptr<GraphicsHandler>& graphicsHandler)
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        InitMemBuffer(bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            graphicsHandler);

    }

    void UniformBuffer::bind()
    {
        
    }

    void UniformBuffer::unbind()
    {
        
    }

}