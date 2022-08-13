#pragma once
#include "Comphi/Renderer/IObjects/IVertexBuffer.h"

namespace Comphi::Vulkan {

	class VertexBuffer : public IVertexBuffer
	{
	public:
		VertexBuffer(const float& vertices, const uint& count);

		virtual void bind() override;
		virtual void unbind() override;
	};
}

