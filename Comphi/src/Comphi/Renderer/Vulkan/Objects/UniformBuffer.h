#pragma once
#include "Comphi/Renderer/IUniformBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class UniformBuffer : public IUniformBuffer,  public MemBuffer
	{
	public:
		UniformBuffer() = default;
		UniformBuffer(const UniformBufferObject& ubo);
		~UniformBuffer() = default;
	};
}