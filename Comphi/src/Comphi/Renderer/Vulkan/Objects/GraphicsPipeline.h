#pragma once
#include "Comphi/Renderer/IGraphicsPipeline.h"

#include "ImageView.h"

namespace Comphi::Vulkan {
	
	//TODO: Differentiate Layout Set Binding IDs between Graphics Pipeline Types
	//Binding points ID do not interfeer with eachother, 
	//each can have their own Set IDs: Graphics, Compute, Ray_tracing (vkPipelineBindPoint)
	
	struct LayoutSet {
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkDescriptorPool> descriptorPools;
	};

	class GraphicsPipeline : public IGraphicsPipeline
	{
	public:

		GraphicsPipeline() = default;
		void initialize(GraphicsPipelineConfiguration config);
		void cleanUp();
	

		void updateDescriptorSet(ShaderResourceDescriptorType type, uint setID, uint descriptorID, void* data);
		void bindDescriptorsToCommandBuffer(uint setID, uint descriptorID);

	private:
		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;

		std::vector<LayoutSet> graphicsSetLayouts;
		
		VkDescriptorSetLayout* getSetLayouts(std::vector<LayoutSet>& setLayoutsObj) {
			std::vector<VkDescriptorSetLayout> setLayouts = std::vector<VkDescriptorSetLayout>(setLayoutsObj.size());
			for (size_t i = 0; i < setLayoutsObj.size(); i++)
			{
				setLayouts[i] = setLayoutsObj[i].descriptorSetLayout;
			}
			return setLayouts.data();
		}
		//std::vector<LayoutSet> ComputeDescriptorSets;
		//std::vector<LayoutSet> RayTracingDescriptorSets;

	};

}