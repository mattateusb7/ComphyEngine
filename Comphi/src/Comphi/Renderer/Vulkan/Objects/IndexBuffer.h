#pragma once
#include "Comphi/Renderer/IIndexBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class IndexBuffer : public IIndexBuffer, public MemBuffer
	{
	public:

		IndexBuffer(const IndexArray& indices);

	};
}