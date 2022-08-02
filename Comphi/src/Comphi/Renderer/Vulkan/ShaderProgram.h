#pragma once
#include "Comphi/Renderer/IShaderProgram.h"
//#include <vulkan/vulkan_core.h>

namespace Comphi::Vulkan {

	class ShaderProgram : public IShaderProgram
	{
	public:
		ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile);
		//~ShaderProgram();
		uint GetType() override;
	};

}