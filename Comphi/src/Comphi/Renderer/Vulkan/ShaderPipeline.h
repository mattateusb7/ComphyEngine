#pragma once
#include "Comphi/Renderer/IShaderPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	class ShaderPipeline : public IShaderPipeline
	{
	public:

		struct GraphicsPipelineSetupData{
			VkViewport* viewport;
			VkRect2D* scissor;
		}graphicsPipelineSetupData;

		ShaderPipeline(GraphicsPipelineSetupData& graphicsPipelineSetupData);
		ShaderPipeline() {};
		bool InitPipeline() override;
		bool BindProgram(IShaderProgram& shaderProgram) override;
		bool UnbindProgram(IShaderProgram& shaderProgram) override;

		std::shared_ptr<VkDevice> logicalDevice;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		std::vector<ShaderProgram*>shaderPrograms;
	};

}