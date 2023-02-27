#pragma once
#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "Comphi/Renderer/Vulkan/Images/ImageView.h"

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
		virtual void initialize() override;

		VkWriteDescriptorSet getDescriptorSetWrite(void* dataObjectsArray, uint setID, uint descriptorID);
		
		virtual void cleanUp() override;

		VkPipeline pipelineObj;
	private:
		VkPipelineLayout pipelineLayout;

		std::vector<LayoutSet> graphicsSetLayouts;

		/*/virtual std::vector<ShaderProgram*> getShaderPrograms() {
			std::vector<ShaderProgram*> list;
			uint stageCount = configuration.pipelineLayoutConfiguration.shaderPrograms.size();
			for (size_t i = 0; i < stageCount; i++)
			{
				ShaderProgram* _shaderProgram = static_cast<ShaderProgram*>(configuration.pipelineLayoutConfiguration.shaderPrograms[i]);
				list.push_back(_shaderProgram);
			}
		}*/

		inline DescriptorSetBinding& getDescriptorSet(uint setID, uint descriptorID) {
			return configuration.pipelineLayoutConfiguration.layoutSets[setID].shaderResourceDescriptorSets[descriptorID];
		}

		std::vector<VkDescriptorSetLayout> getSetLayouts(std::vector<LayoutSet>& setLayoutsObj) {
			std::vector<VkDescriptorSetLayout> setLayouts = std::vector<VkDescriptorSetLayout>(setLayoutsObj.size());
			for (size_t i = 0; i < setLayoutsObj.size(); i++)
			{
				setLayouts[i] = setLayoutsObj[i].descriptorSetLayout;
			}
			return setLayouts;
		}
		//std::vector<LayoutSet> ComputeDescriptorSets;
		//std::vector<LayoutSet> RayTracingDescriptorSets;

	};

}