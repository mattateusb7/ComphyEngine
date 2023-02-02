#include "cphipch.h"
#include "Material.h"

namespace Comphi::Vulkan {

	Material::Material(Comphi::MaterialProperties properties)
	{
		shaderTextures = properties.shaderTextures;

		for (int i = 0; i < properties.shaderPrograms.size(); i++) {
			BindProgram(*properties.shaderPrograms[i]);
		}

		graphicsPipeline.initialize(shaderStages);

		for (int i = 0; i < properties.shaderPrograms.size(); i++) {
			UnbindProgram(*properties.shaderPrograms[i]); 
		}
	}

	bool Material::BindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<Vulkan::ShaderProgram*> (&shaderProgram); //Casting Interface

		switch (shaderProgram.GetType())
		{
		case (uint)Comphi::ShaderType::VertexShader: {
			//VERTEX
			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = _shaderProgram->shaderModule;
			vertShaderStageInfo.pName = "main";
			vertShaderStageInfo.pSpecializationInfo = nullptr;
			shaderStages.push_back(vertShaderStageInfo);
			break;
		}
		case (uint)Comphi::ShaderType::FragmentShader: {
			//FRAGMENT
			VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = _shaderProgram->shaderModule;
			fragShaderStageInfo.pName = "main";
			shaderStages.push_back(fragShaderStageInfo);
			break;
		}
		default:
			break;
		}

		shaderPrograms.push_back(_shaderProgram);

		return true;
	}

	bool Material::UnbindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<Vulkan::ShaderProgram*>(&shaderProgram);

		auto it_shaderProgram = std::find(shaderPrograms.begin(), shaderPrograms.end(), _shaderProgram);
		if (it_shaderProgram != shaderPrograms.end()) {
			shaderPrograms.erase(it_shaderProgram);
			return true;
		}
		return false;
	}

	void Material::sendDescriptorSet(std::vector<UniformBuffer>& MVP_ubos)
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
	}

	void Material::cleanUp()
	{
		graphicsPipeline.cleanUp();
	}
	
}

