#pragma once
#include "Comphi/Renderer/IObjects/IUniformBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class UniformBuffer : public MemBuffer
	{
	public:
		UniformBuffer() = default;
		UniformBuffer(const UniformBufferObject& ubo);
		~UniformBuffer() = default;
	};
}