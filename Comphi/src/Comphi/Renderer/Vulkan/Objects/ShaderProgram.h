#pragma once
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"
#include "Comphi/Renderer/IShaderProgram.h"
#include "Comphi/Platform/IFileRef.h"

namespace Comphi::Vulkan {

	struct shaderStageCreateInfo {

	};

	class ShaderProgram : public IShaderProgram
	{
	public:
		ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile);
		VkShaderModule shaderModule;
		VkPipelineShaderStageCreateInfo shaderStageInfo{};

		//bool operator==(ShaderProgram& other) {
		//	return other. GetType() == GetType();
		//}
		virtual void cleanUp() override;
	private:
		void createShaderModule();

	};

}