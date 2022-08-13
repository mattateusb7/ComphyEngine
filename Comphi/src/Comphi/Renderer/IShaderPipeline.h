#pragma once
#include "IShaderProgram.h"

namespace Comphi {
	class IShaderPipeline
	{
	public:
		virtual bool InitPipeline() = 0;
		virtual bool BindProgram(IShaderProgram& shaderProgram) = 0;
		virtual bool UnbindProgram(IShaderProgram& shaderProgram) = 0;
	};
}