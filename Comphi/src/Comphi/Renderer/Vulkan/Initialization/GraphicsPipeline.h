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

		GraphicsPipeline(std::vector<VkPipelineShaderStageCreateInfo>& shaderStages);

		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;

		//TODO: This should be referenced from outside 
		std::unique_ptr<DescriptorPool> descriptorPool;

		//VkDescriptorSet;

		~GraphicsPipeline();
	};

}