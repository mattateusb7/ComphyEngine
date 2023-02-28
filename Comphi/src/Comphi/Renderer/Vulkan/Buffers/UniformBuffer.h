#pragma once
#include "MemBuffer.h"
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi::Vulkan {

    template<typename T>
    class UniformBuffer : public IUniformBuffer, public MemBuffer
    {
    public:
        UniformBuffer(const T* dataArray, const uint count, BufferUsage usage = BufferUsage::UniformBuffer);
        //Initialize(const T* dataArray, const uint count, BufferUsage usage = BufferUsage::UniformBuffer);
        virtual void updateBufferData(const void* dataArray) override;
        virtual void cleanUp() override { static_cast<MemBuffer*>(this)->cleanUp(); }
    private :
        void copyData(const MemBuffer& membuffer, const void* dataArray);
    };

    template<typename T>
    UniformBuffer<T>::UniformBuffer(const T* dataArray, const uint count, BufferUsage usage)
    {
        bufferSize = sizeof(dataArray[0]) * count;
        bufferUsage = usage;

        VkBufferUsageFlagBits usageFlags;
        VkMemoryPropertyFlags accessFlags;
        switch (bufferUsage)
        {
        case BufferUsage::UniformBuffer:
            usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            accessFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            break;
        case BufferUsage::VertexBuffer:
            usageFlags = VkBufferUsageFlagBits(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            accessFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; 
            break;
        case BufferUsage::IndexBuffer:
            usageFlags = VkBufferUsageFlagBits(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            accessFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        default:
            break;
        }

        if (bufferUsage == BufferUsage::UniformBuffer) {
            allocateMemoryBuffer(bufferSize,
                usageFlags, accessFlags);
            return;
        }

        allocateMemoryBuffer(bufferSize,
            usageFlags, accessFlags);

        updateBufferData(dataArray);
    }

    template<typename T>
    void Comphi::Vulkan::UniformBuffer<T>::updateBufferData(const void* dataArray)
    {
        if (bufferUsage == BufferUsage::UniformBuffer) {
            const MemBuffer* this_membuffer = this;
            copyData(*this_membuffer, dataArray);
            return;
        }

        MemBuffer stagingBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        copyData(stagingBuffer, dataArray);

        MemBuffer::copyBufferTo(stagingBuffer.bufferObj, bufferObj, (size_t)bufferSize);

        //cleanup
        stagingBuffer.cleanUp();
    }

    template<typename T>
    void UniformBuffer<T>::copyData(const MemBuffer& membuffer, const void* dataArray)
    {
        void* deviceMemoryData;
        vkMapMemory(GraphicsHandler::get()->logicalDevice, membuffer.bufferMemory, 0, membuffer.bufferSize, 0, &deviceMemoryData);
        memcpy(deviceMemoryData, dataArray, (size_t)bufferSize);
        vkUnmapMemory(GraphicsHandler::get()->logicalDevice, membuffer.bufferMemory);
    }
}