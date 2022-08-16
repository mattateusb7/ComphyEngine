#pragma once
#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "GraphicsContext.h"

namespace Comphi::Vulkan {

	class ShaderProgram;

	class GraphicsPipeline : public IGraphicsPipeline
	{
	public:

		struct GraphicsPipelineSetupData{
			VkViewport* viewport;
			VkRect2D* scissor;
		}graphicsPipelineSetupData;

		GraphicsPipeline(GraphicsPipelineSetupData& graphicsPipelineSetupData);
		~GraphicsPipeline();
		bool InitPipeline() override;
		bool BindProgram(IShaderProgram& shaderProgram) override;
		bool UnbindProgram(IShaderProgram& shaderProgram) override;

		void createDescriptorSetLayout();

		std::shared_ptr<VkDevice> logicalDevice;
		VkRenderPass renderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		std::vector<ShaderProgram*>shaderPrograms;
	};

}