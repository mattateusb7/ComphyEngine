#pragma once
#include "Comphi/Renderer/IObjects/IIndexBuffer.h"
#include "../GraphicsHandler.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class IndexBuffer : public IIndexBuffer
	{
	public:

		IndexBuffer(const IndexArray& indices, GraphicsHandler& graphicsHandler);
		~IndexBuffer() = default;
		std::unique_ptr<MemBuffer> buffer;
		uint32_t indexCount;

		virtual void bind() override;
		virtual void unbind() override;
	};
}