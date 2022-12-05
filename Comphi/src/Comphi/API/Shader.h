#pragma once
#include "Comphi/Renderer/IShaderProgram.h"
namespace Comphi {

	class Shader : IShaderProgram
	{
		Shader(Comphi::ShaderType shaderType, IFileRef& shaderFile);
	};

};

