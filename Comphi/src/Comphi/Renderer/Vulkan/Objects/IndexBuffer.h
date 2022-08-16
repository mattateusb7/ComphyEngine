#pragma once
#include "Comphi/Renderer/IObjects/IIndexBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	struct GraphicsHandler;
	class MemBuffer;

	class IndexBuffer : public IIndexBuffer
	{
	public:

		IndexBuffer(const IndexArray& indices, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		~IndexBuffer() = default;
		std::unique_ptr<MemBuffer> buffer;
		uint32_t indexCount;

		virtual void bind() override;
		virtual void unbind() override;
	};
}