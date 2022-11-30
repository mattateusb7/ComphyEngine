#include "cphipch.h"
#include "Material.h"

namespace Comphi {

	Material::Material(MaterialProperties properties)
	{
		shaderTextures = properties.textures;

		for (int i = 0; i < properties.shaders.size(); i++) {
			BindProgram(*properties.shaders[i]);
		}

		graphicsPipeline.initialize(shaderStages, *properties.descriptorPool);

		for (int i = 0; i < properties.shaders.size(); i++) {
			UnbindProgram(*properties.shaders[i]);
		}
	}

	bool Material::BindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<Vulkan::ShaderProgram* > (&shaderProgram); //Interfaces Need Rework

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
			COMPHILOG_CORE_INFO("Destroyed ShaderModule!");
			vkDestroyShaderModule(*Vulkan::GraphicsHandler::get()->logicalDevice, (*it_shaderProgram)->shaderModule, nullptr);
			shaderPrograms.erase(it_shaderProgram);
			return true;
		}
		return false;
	}

	Material::~Material()
	{
		graphicsPipeline.~GraphicsPipeline();
	}
	
}

