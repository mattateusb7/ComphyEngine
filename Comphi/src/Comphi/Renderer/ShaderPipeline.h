#pragma once
#include "ShaderProgram.h"

namespace Comphi {
	class ShaderPipeline
	{
	public:
		ShaderPipeline() = default;
		virtual ~ShaderPipeline() = default;
		virtual bool InitPipeline() = 0;
		virtual bool BindProgram(ShaderProgram& shaderProgram) = 0;
		virtual bool UnbindProgram(ShaderProgram& shaderProgram) = 0;
		virtual bool BindPipeline() = 0;
	protected:
		uint m_pipeline = -1;
	};
}