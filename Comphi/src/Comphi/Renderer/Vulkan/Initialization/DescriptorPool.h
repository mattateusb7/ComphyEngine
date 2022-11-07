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
		VkDescriptorPool descriptorPoolObj; //TODO: In the future share multiple sets with least ammount of pools (static?) 
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
		void bindDescriptorSet(std::vector<Vulkan::Texture*>textures, std::vector<UniformBuffer> MVP_ubos);
		~DescriptorPool();
	protected:
		void createDescriptorPool();
		void createDescriptorSetLayout(); //TODO: Layout TBD by shaderPipeline

	};
}
