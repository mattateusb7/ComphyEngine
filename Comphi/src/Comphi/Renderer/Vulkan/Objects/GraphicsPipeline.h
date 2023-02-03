#pragma once
#include "../Objects/UniformBuffer.h"
#include "../Objects/ShaderProgram.h"
#include "Comphi/Renderer/IGraphicsPipeline.h"

#include "ImageView.h"

namespace Comphi::Vulkan {

	class GraphicsPipeline : public IGraphicsPipeline
	{
	public:

		//struct GraphicsPipelineSetupData{
		//	VkViewport* viewport;
		//	VkRect2D* scissor;
		//}graphicsPipelineSetupData;

		GraphicsPipeline() = default;
		void initialize(GraphicsPipelineConfiguration config);
		
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;  
		std::vector<VkDescriptorPool> descriptorPools;

		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;

		//std::vector<VkDescriptorSet> descriptorSets;

		//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts
		//Or just dynamic Binding for construction of those structs
		//void sendDescriptorSet(std::vector<ITexture*>& textures, std::vector<UniformBuffer>& MVP_ubos);
		
		void cleanUp();
	


	protected:
		//void createDescriptorPool();
		//void createDescriptorSetLayout();
		////void createPipelineLayout();
	};

}