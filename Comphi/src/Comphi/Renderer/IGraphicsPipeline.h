#pragma once

namespace Comphi {

	class IShaderProgram;
	class IGraphicsPipeline
	{
	public:
		virtual bool InitPipeline() = 0;
		virtual bool BindProgram(IShaderProgram& shaderProgram) = 0;
		virtual bool UnbindProgram(IShaderProgram& shaderProgram) = 0;
	};
}