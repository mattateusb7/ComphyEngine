#pragma once
#include "../Objects/ShaderProgram.h"
#include "DescriptorPool.h"

namespace Comphi::Vulkan {

	class GraphicsPipeline
	{
	public:

		//struct GraphicsPipelineSetupData{
		//	VkViewport* viewport;
		//	VkRect2D* scissor;
		//}graphicsPipelineSetupData;

		GraphicsPipeline() = default;
		void initialize(std::vector<VkPipelineShaderStageCreateInfo>& shaderStages, DescriptorPool& descriptorPool);

		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;
		std::vector<VkDescriptorSet> descriptorSets;

		//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts
		void sendDescriptorSet(std::vector<Vulkan::Texture*>& textures, std::vector<UniformBuffer>& MVP_ubos);

		~GraphicsPipeline();
	
	protected:
		DescriptorPool* updatePool;
		void createDescriptorSetLayout();
		VkDescriptorSetLayout descriptorSetLayout;
	};

}