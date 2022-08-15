#pragma once
#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "ShaderProgram.h"
#include "Objects/VertexBuffer.h"

namespace Comphi::Vulkan {

	class GraphicsPipeline : public IGraphicsPipeline
	{
	public:

		struct GraphicsPipelineSetupData{
			VkViewport* viewport;
			VkRect2D* scissor;
		}graphicsPipelineSetupData;

		GraphicsPipeline(GraphicsPipelineSetupData& graphicsPipelineSetupData);
		GraphicsPipeline();
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