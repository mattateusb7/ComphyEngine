#pragma once
#include "../ShaderWizard.h"
#include <glad\glad.h>

namespace Comphi {
	static class OpenGLShaderWizard : ShaderWizard
	{
	public:
		static bool CompileShader(ShaderProgram& shaderProgram) { 
			OpenGLShaderWizard wiz;
			return wiz.Compile(shaderProgram); 
		};
	private:
		bool Compile(ShaderProgram& shaderProgram) override;
		bool CheckCompileStatus(uint shaderID) override;
		bool CheckLinkStatus(uint programID) override;
		bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	};
}
