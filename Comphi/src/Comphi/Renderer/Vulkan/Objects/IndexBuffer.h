#pragma once
#include "Comphi/Renderer/IObjects/IIndexBuffer.h"

namespace Comphi::Vulkan {

	class IndexBuffer : public IIndexBuffer
	{
	public:
		IndexBuffer(const uint& indices);
		uint ID;

		virtual void bind() override;
		virtual void unbind() override;
	};
}