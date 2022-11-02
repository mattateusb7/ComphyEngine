#pragma once
#include "../GraphicsHandler.h"
#include "RenderPass.h"

namespace Comphi::Vulkan {

	class ShaderProgram;

	class GraphicsPipeline
	{
	public:

		//struct GraphicsPipelineSetupData{
		//	VkViewport* viewport;
		//	VkRect2D* scissor;
		//}graphicsPipelineSetupData;

		GraphicsPipeline(RenderPass& renderPass, std::vector<VkPipelineShaderStageCreateInfo> shaderStages);
		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;
		~GraphicsPipeline();
	};

}