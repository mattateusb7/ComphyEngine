#pragma once
#include "MemBuffer.h"
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi::Vulkan {

    class UniformBuffer : public IUniformBuffer, public MemBuffer
    {
    public:
        UniformBuffer(const void* dataArray, const uint size, const uint count, BufferUsage usage = BufferUsage::UniformBuffer);
        //Initialize(const T* dataArray, const uint count, BufferUsage usage = BufferUsage::UniformBuffer);
        virtual void updateBufferData(const void* dataArray) override;
        virtual void cleanUp() override { static_cast<MemBuffer*>(this)->cleanUp(); }
    private :
        void copyData(const MemBuffer& membuffer, const void* dataArray);
    };

   
}