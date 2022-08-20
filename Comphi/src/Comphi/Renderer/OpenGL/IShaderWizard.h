#pragma once
#include "../IShaderProgram.h"

namespace Comphi {
	class IShaderWizard
	{
	private:
		virtual bool Compile(IShaderProgram& shaderProgram) = 0;
		virtual bool CheckCompileStatus(uint shaderID) = 0;
		virtual bool CheckLinkStatus(uint programID) = 0;
	};
}
