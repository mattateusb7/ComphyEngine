#pragma once
#include "../Objects/UniformBuffer.h"
#include "ImageView.h"

namespace Comphi::Vulkan {

	class GraphicsPipeline
	{
	public:

		//struct GraphicsPipelineSetupData{
		//	VkViewport* viewport;
		//	VkRect2D* scissor;
		//}graphicsPipelineSetupData;

		GraphicsPipeline() = default;
		//TODO: Swap this shaderStages with ShaderStagesPipeline Class (also defining deescriptor Sets)
		void initialize(std::vector<VkPipelineShaderStageCreateInfo>& shaderStages);

		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;

		std::vector<VkDescriptorSet> descriptorSets;
		VkDescriptorPool descriptorPoolObj;

		//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts
		void sendDescriptorSet(std::vector<ITexture*>& textures, std::vector<UniformBuffer>& MVP_ubos);

		void cleanUp();
	
	protected:
		void createDescriptorPool();
		void createDescriptorSetLayout();
		//void createPipelineLayout();
		VkDescriptorSetLayout descriptorSetLayout;
	};

}