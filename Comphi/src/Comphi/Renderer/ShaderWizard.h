#pragma once
#include "ShaderProgram.h"

namespace Comphi {
	class ShaderWizard
	{
	public:
		ShaderWizard() {};
	protected:
		virtual bool Compile(ShaderProgram& shaderProgram) = 0;
	private:
		virtual bool CheckCompileStatus(uint shaderID) = 0;
		virtual bool CheckLinkStatus(uint programID) = 0;
	};
}
