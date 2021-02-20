#pragma once
#include "../ShaderWizard.h"
#include <glad\glad.h>

namespace Comphi {
	class OpenGLShaderWizard : ShaderWizard
	{
	public:
		bool Compile(ShaderProgram& shaderProgram) override;
	private:
		bool CheckCompileStatus(uint shaderID) override;
		bool CheckLinkStatus(uint programID) override;
		bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	};
}
