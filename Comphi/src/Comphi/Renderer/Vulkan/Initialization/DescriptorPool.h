#pragma once
#include "../GraphicsHandler.h"
#include <Comphi/Core/API/MeshObject.h>

namespace Comphi::Vulkan {

	//RenderPass Descriptor
	class DescriptorPool
	{
	protected:

	public:
		DescriptorPool(int MAX_FRAMES_IN_FLIGHT);
		VkDescriptorPool descriptorPoolObj;
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;

		void updateDescriptorSet(MeshObject obj, int MAX_FRAMES_IN_FLIGHT);
	protected:
		void createDescriptorPool(int MAX_FRAMES_IN_FLIGHT);
		void createDescriptorSetLayout();

	};
}
