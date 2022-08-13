#pragma once
#include <glad\glad.h>
#include "../IShaderPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::OpenGL{

	class ShaderPipeline : public IShaderPipeline
	{
	public:
		ShaderPipeline();
		~ShaderPipeline();
		bool InitPipeline();
		bool BindProgram(IShaderProgram& shaderProgram) override;
		bool UnbindProgram(IShaderProgram& shaderProgram) override;
		bool BindPipeline();
	protected:
		uint m_pipeline = -1;
	};
	
}