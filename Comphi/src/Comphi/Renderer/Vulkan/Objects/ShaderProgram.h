#pragma once
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"
#include "Comphi/Renderer/IShaderProgram.h"
#include "Comphi/Platform/Windows/FileRef.h"

namespace Comphi::Vulkan {

	class ShaderProgram : public IShaderProgram
	{
	public:
		ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile);
		VkShaderModule shaderModule;

		VkShaderModule recreateShaderModule();

		//bool operator==(ShaderProgram& other) {
		//	return other.GetType() == GetType();
		//}
		~ShaderProgram();
	protected:
		VkShaderModule createShaderModule(const std::vector<char>& code);
	};

#define ShaderProgramInstance std::shared_ptr<ShaderProgram>
#define MakeShaderProgramInstance std::make_shared<ShaderProgram>

}