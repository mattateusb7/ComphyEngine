#pragma once
#include "IShaderProgram.h"

namespace Comphi {
	class IShaderPipeline
	{
	public:
		IShaderPipeline() = default;
		virtual ~IShaderPipeline() = default;
		virtual bool InitPipeline() = 0;
		virtual bool BindProgram(IShaderProgram& shaderProgram) = 0;
		virtual bool UnbindProgram(IShaderProgram& shaderProgram) = 0;
		virtual bool BindPipeline() = 0;
	protected:
		uint m_pipeline = -1;
	};
}