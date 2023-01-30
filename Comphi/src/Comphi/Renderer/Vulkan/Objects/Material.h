#pragma once

#include "Comphi/Renderer/IObject.h"
#include "Comphi/Renderer/IMaterial.h"
#include "GraphicsPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	class Material : public IMaterial
	{
	public:
		
		Material(Comphi::MaterialProperties properties); // Descriptor Sets : https://youtu.be/5VBVWCg7riQ?t=1171
		//Binding IDS ?
		//Binding points ID do not interfeer with eachother, each has their own IDs: Graphics, Compute, Ray_tracing (vkPipelineBindPoint)
		void cleanUp();

		//Each Material owns one Shader/Graphics Pipeline 
		GraphicsPipeline graphicsPipeline;

		//linked MaterialProperties:
		std::vector<ShaderProgram*> shaderPrograms = std::vector<ShaderProgram*>();

		void bindGraphicsPipeline(VkCommandBuffer& commandBuffer);
		void bindDescriptorSet(VkCommandBuffer& commandBuffer, uint32_t currentFrame);
		void sendDescriptorSet(std::vector<UniformBuffer>& MVP_ubos);

	protected:

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		bool BindProgram(IShaderProgram& shaderProgram);
		bool UnbindProgram(IShaderProgram& shaderProgram);

	};

}

