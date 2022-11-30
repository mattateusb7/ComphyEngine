#pragma once
#include "../GraphicsHandler.h"
#include "Comphi/Renderer/Vulkan/Objects/ImageView.h"
#include <Comphi/Renderer/Vulkan/Objects/UniformBuffer.h>

namespace Comphi::Vulkan {

	//shader Descriptor set immeddiatly before renderpass
	class DescriptorPool
	{
	protected:

	public:
		DescriptorPool();
		VkDescriptorPool descriptorPoolObj;
		~DescriptorPool();
	};
	#define DescriptorPoolInstance std::shared_ptr<DescriptorPool>
	#define MakeDescriptorPoolInstance std::make_shared<DescriptorPool>
}
