#pragma once
#include "Comphi/Renderer/IUniformBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class UniformBuffer : public UniformBufferObject,  public MemBuffer
	{
	public:
		UniformBuffer();
		~UniformBuffer() = default;
	};
}