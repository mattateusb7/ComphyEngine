#include "cphipch.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	ShaderProgram::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile) : IShaderProgram(shaderType, shaderFile) {
		createShaderModule();
		initializeShaderStageInfo();
	}

	void ShaderProgram::createShaderModule() {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderFile.getByteData().size();
		createInfo.pCode = shaderFile.getUint32tByteData();

		vkCheckError(vkCreateShaderModule(GraphicsHandler::get()->logicalDevice, &createInfo, nullptr, &shaderModule)) {
			COMPHILOG_CORE_FATAL("failed to create shader module!");
		}
		COMPHILOG_CORE_INFO("created shaderModule successfully!");
		
	}

	void ShaderProgram::cleanUp() {
		vkDestroyShaderModule(GraphicsHandler::get()->logicalDevice, shaderModule, 0);
		COMPHILOG_CORE_INFO("shaderModule Destroyed!");
	}
	
	void ShaderProgram::initializeShaderStageInfo()
	{
		switch (GetType())
		{
		case (uint)Comphi::ShaderType::VertexShader: {
			//VERTEX
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			shaderStageInfo.module = shaderModule;
			shaderStageInfo.pName = "main";
			shaderStageInfo.pSpecializationInfo = nullptr;
			break;
		}
		case (uint)Comphi::ShaderType::FragmentShader: {
			//FRAGMENT
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			shaderStageInfo.module = shaderModule;
			shaderStageInfo.pName = "main";
			break;
		}
		default:
			break;
		}
	}

}