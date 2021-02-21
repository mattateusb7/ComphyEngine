#pragma once
#include "ShaderProgram.h"

namespace Comphi {
	class ShaderWizard
	{
	private:
		virtual bool Compile(ShaderProgram& shaderProgram) = 0;
		virtual bool CheckCompileStatus(uint shaderID) = 0;
		virtual bool CheckLinkStatus(uint programID) = 0;
	};
}
