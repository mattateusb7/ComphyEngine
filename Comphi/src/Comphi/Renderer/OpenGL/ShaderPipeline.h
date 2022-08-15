#pragma once
#include <glad\glad.h>
#include "../IGraphicsPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::OpenGL{

	class ShaderPipeline : public IGraphicsPipeline
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