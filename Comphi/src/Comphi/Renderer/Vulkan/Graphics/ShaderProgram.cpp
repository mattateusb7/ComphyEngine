#include "cphipch.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	ShaderProgram::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile) : IShaderProgram(shaderType, shaderFile) {
		
		//createShaderModule
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

}