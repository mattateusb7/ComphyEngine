#pragma once
#include "Comphi/Renderer/IShaderProgram.h"
#include "../Common.h"
#include "Comphi/Platform/Windows/FileRef.h"

namespace Comphi::Vulkan {

	class ShaderProgram : public IShaderProgram
	{
	public:
		ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile, const VkDevice& logicalDevice);
		VkShaderModule shaderModule;
		std::shared_ptr<VkDevice> logicalDevice;

		VkShaderModule recreateShaderModule(const VkDevice& logicalDevice);

		//bool operator==(ShaderProgram& other) {
		//	return other.GetType() == GetType();
		//}

	protected:
		VkShaderModule createShaderModule(const std::vector<char>& code);
	};

}