#pragma once
#include "../IShaderPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	class ShaderPipeline : public IShaderPipeline
	{
	public:
		ShaderPipeline();
		~ShaderPipeline() override;
		bool InitPipeline() override;
		bool BindProgram(IShaderProgram& shaderProgram) override;
		bool UnbindProgram(IShaderProgram& shaderProgram) override;
		bool BindPipeline() override;
	};

}