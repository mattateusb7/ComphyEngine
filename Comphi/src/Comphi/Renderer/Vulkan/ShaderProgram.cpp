#include "cphipch.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	ShaderProgram::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile, const VkDevice& logicalDevice) : IShaderProgram(shaderType, shaderFile) {
		shaderModule = recreateShaderModule(logicalDevice);
	}

	VkShaderModule ShaderProgram::createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(*logicalDevice.get(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create shader module!");
			return shaderModule;
		}
		COMPHILOG_CORE_INFO("created shaderModule successfully!");

		return shaderModule;
	}

	VkShaderModule ShaderProgram::recreateShaderModule(const VkDevice& logicalDevice)
	{
		this->logicalDevice = std::make_shared<VkDevice>(logicalDevice);
		shaderModule = createShaderModule(shaderFile.getByteData());
		return shaderModule;
	}
	

}