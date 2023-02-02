#include "cphipch.h"
#include "Material.h"

namespace Comphi::Vulkan {

	Material::Material(MaterialResources& properties)
	{
		for (int i = 0; i < properties.shaderPrograms.size(); i++) {
			bindShaderProgram(*properties.shaderPrograms[i]);
		}

		graphicsPipeline.initialize(properties.pipelineConfiguration);

		this->properties = properties;
	}

	bool Material::bindShaderProgram(IShaderProgram& shaderProgram)
	{
		ShaderProgram* _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram); //API Abstraction
		shaderPrograms.push_back(_shaderProgram);
	}

	bool Material::unbindShaderProgram(IShaderProgram& shaderProgram)
	{
		ShaderProgram* _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram); //API Abstraction

		auto it_shaderProgram = std::find(shaderPrograms.begin(), shaderPrograms.end(), _shaderProgram);
		if (it_shaderProgram != shaderPrograms.end()) {
			shaderPrograms.erase(it_shaderProgram);
			return true;
		}
		return false;
	}

	/*void Material::sendDescriptorSet(std::vector<UniformBuffer>& MVP_ubos)
	{
		graphicsPipeline.sendDescriptorSet(shaderTextures, MVP_ubos);
	}

	void Material::bindGraphicsPipeline(VkCommandBuffer& commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineObj);
	}

	void Material::bindDescriptorSet(VkCommandBuffer& commandBuffer, uint32_t currentFrame)
	{
		//TODO: Refactor move to command Buffer Pool ? : https://youtu.be/J1A1kWBVrzc?t=779
		//Binding the command buffers to descriptor set will be a client side operation since its the user who decides what resources to bind to what commandBuffer 
		//The descriptor sets become static on each specific queue and holding its own ID
		//when sending batch command buffer operation we can bind them to these IDs
		// 
		//Maybe we can try to setup the whole command Buffer sequence pre-draw and only update the DescriptorSet's resources every frame, updating the data with the vkCmdBindDescriptorSets 
		//for reusing command buffers we'd need to set the command buffer usage Mode to "reuse" or "reset and re-record"
		//But we need to be aware that if the instructions executed change acordingly to an application's requirements
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &graphicsPipeline.descriptorSets[currentFrame], 0, nullptr);
	}*/

	void Material::cleanUp()
	{

		for (int i = 0; i < properties.shaderPrograms.size(); i++) {
			UnbindProgram(*properties.shaderPrograms[i]);
		}
		graphicsPipeline.cleanUp();
	}
	
}

