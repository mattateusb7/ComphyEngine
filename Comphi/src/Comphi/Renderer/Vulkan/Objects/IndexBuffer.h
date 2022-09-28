#pragma once
#include "Comphi/Renderer/IObjects/IIndexBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class IndexBuffer : public MemBuffer
	{
	public:

		IndexBuffer(const IndexArray& indices);
		~IndexBuffer() = default;
		uint32_t indexCount;

	};
}