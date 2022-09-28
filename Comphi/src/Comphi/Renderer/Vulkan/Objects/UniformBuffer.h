#pragma once
#include "Comphi/Renderer/IObjects/IUniformBuffer.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class UniformBuffer : public IUniformBuffer, public MemBuffer
	{
	public:

		UniformBuffer(const UniformBufferObject& ubo);
		~UniformBuffer() = default;

		virtual void bind() override;
		virtual void unbind() override;
	};
}