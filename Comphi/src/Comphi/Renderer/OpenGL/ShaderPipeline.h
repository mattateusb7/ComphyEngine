#pragma once
#include <glad\glad.h>
#include "../IShaderPipeline.h"

namespace Comphi::OpenGL{

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