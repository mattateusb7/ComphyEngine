#include "cphipch.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	ShaderProgram::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile) : IShaderProgram(shaderType, shaderFile) {
		shaderModule = recreateShaderModule();
	}

	VkShaderModule ShaderProgram::createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(*GraphicsHandler::get()->logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create shader module!");
			return shaderModule;
		}
		COMPHILOG_CORE_INFO("created shaderModule successfully!");

		return shaderModule;
	}

	VkShaderModule ShaderProgram::recreateShaderModule()
	{
		shaderModule = createShaderModule(shaderFile.getByteData());
		return shaderModule;
	}

	ShaderProgram::~ShaderProgram() {
		vkDestroyShaderModule(*GraphicsHandler::get()->logicalDevice, shaderModule, 0);
		COMPHILOG_CORE_INFO("shaderModule Destroyed!");
	}
	

}