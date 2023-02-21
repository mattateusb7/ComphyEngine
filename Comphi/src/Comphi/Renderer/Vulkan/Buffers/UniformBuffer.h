#pragma once
#include "MemBuffer.h"

namespace Comphi::Vulkan {

    template<typename T>
    class UniformBuffer : public MemBuffer
    {
    public:
        UniformBuffer(const T* dataArray, const uint count, BufferUsage usage = BufferUsage::UniformBuffer);
    };
}