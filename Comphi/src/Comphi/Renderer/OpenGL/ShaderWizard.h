#pragma once
#include "../IShaderWizard.h"
#include <glad\glad.h>

namespace Comphi::OpenGL {
	class ShaderWizard : IShaderWizard
	{
	public:
		static bool CompileShader(IShaderProgram& shaderProgram) {
			static ShaderWizard wiz;
			return wiz.Compile(shaderProgram);
		};
	private:
		bool Compile(IShaderProgram& shaderProgram) override;
		bool CheckCompileStatus(uint shaderID) override;
		bool CheckLinkStatus(uint programID) override;
		bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	};
}
