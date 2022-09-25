#pragma once
#include <glad\glad.h>
#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::OpenGL{

	class ShaderPipeline : public IGraphicsPipeline
	{
	public:
		ShaderPipeline();
		~ShaderPipeline();
		bool InitPipeline() override;
		bool BindProgram(IShaderProgram& shaderProgram) override;
		bool UnbindProgram(IShaderProgram& shaderProgram) override;
		bool BindPipeline();
	protected:
		uint m_pipeline = -1;
	};
	
}