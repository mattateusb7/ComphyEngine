#pragma once

#include "Comphi/Allocation/IObject.h"
#include "Comphi/Renderer/IMaterial.h"
#include "GraphicsPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	//API Abstraction
	typedef std::vector<ITexture*> ShaderTextures;
	typedef std::vector<IShaderProgram*> ShaderPrograms;

	struct MaterialResources
	{

		GraphicsPipelineConfiguration pipelineConfiguration;

		//TODO: DescriptorSets containing references to commandBuffer resources (Shader accessible data)
		// Descriptor Sets : https://youtu.be/5VBVWCg7riQ?t=117

		ShaderPrograms shaderPrograms = ShaderPrograms();
		ShaderTextures shaderTextures = ShaderTextures();
		//uniform MVPMatrix 
		//Uniforms
		//All kinds of data
		
		//Binding IDS ?
		//Binding points ID do not interfeer with eachother, each has their own IDs: Graphics, Compute, Ray_tracing (vkPipelineBindPoint)
	};

	class Material : public IMaterial
	{
	public:

		Material(MaterialResources& properties); 1
		virtual void cleanUp() override;

		MaterialResources properties;

		//Each Material owns one Shader/Graphics Pipeline 
		GraphicsPipeline graphicsPipeline;

		//void bindGraphicsPipeline(VkCommandBuffer& commandBuffer);
		//void bindDescriptorSet(VkCommandBuffer& commandBuffer, uint32_t currentFrame);
		//void sendDescriptorSet(std::vector<UniformBuffer>& MVP_ubos);

	protected:

		bool bindShaderProgram(IShaderProgram& shaderProgram);
		bool unbindShaderProgram(IShaderProgram& shaderProgram);
		
	};

}

