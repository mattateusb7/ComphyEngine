#pragma once
#include "Comphi/Renderer/IObjects/IIndexBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class IndexBuffer : public IIndexBuffer, public MemBuffer
	{
	public:

		IndexBuffer(const IndexArray& indices, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		~IndexBuffer() = default;
		uint32_t indexCount;

		virtual void bind() override;
		virtual void unbind() override;
	};
}