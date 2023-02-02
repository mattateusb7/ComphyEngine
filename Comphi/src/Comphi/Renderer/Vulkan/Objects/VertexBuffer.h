#pragma once
#include "Comphi/Renderer/IVertexBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class VertexBuffer : public IVertexBuffer, public MemBuffer
	{
	public:

		VertexBuffer(const VertexArray& vertices);

	};
}

