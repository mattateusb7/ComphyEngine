#include "cphipch.h"
#include "UniformBuffer.h"

namespace Comphi::Vulkan {

    UniformBuffer::UniformBuffer()
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        InitMemBuffer(bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    }
}