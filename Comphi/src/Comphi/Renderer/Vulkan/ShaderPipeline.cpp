#include "cphipch.h"
#include "ShaderPipeline.h"

namespace Comphi::Vulkan {

	ShaderPipeline::ShaderPipeline() {
		// Init pipeline
	}

	ShaderPipeline::~ShaderPipeline()
	{
	}

	bool ShaderPipeline::InitPipeline()
	{
		return true;
	}

	bool ShaderPipeline::BindProgram(IShaderProgram& shaderProgram)
	{
		return true;
	}

	bool ShaderPipeline::UnbindProgram(IShaderProgram& shaderProgram)
	{
		return true;
	}

	bool ShaderPipeline::BindPipeline()
	{
		return true;
	}

}