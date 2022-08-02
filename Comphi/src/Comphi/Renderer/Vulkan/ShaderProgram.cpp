#include "cphipch.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	ShaderProgram::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile) : IShaderProgram(shaderType, shaderFile) {



	}

	uint ShaderProgram::GetType() {
		return 0;
	}
	

}