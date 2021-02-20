#include "cphipch.h"
#include "OpenGLShaderWizard.h"

namespace Comphi {
	bool OpenGLShaderWizard::Compile(ShaderProgram& shaderProgram)
	{
		if (shaderProgram.shaderID == -1) return false;

		//glCreateShaderProgramv ?auto setup?
		shaderProgram.shaderID = glCreateProgram();

		// Declare that program is separable
		glProgramParameteri(shaderProgram.shaderID, GL_PROGRAM_SEPARABLE, GL_TRUE);

		GLuint shader = glCreateShader(shaderProgram.GetType());

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)shaderProgram.shaderSource.c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);
		/*if (!CheckCompileStatus(shader)) {
			shaderProgram.shaderID = -1;
			return false;
		}*/
		// Shader compilation is successful.

		// Attach the shader to its respective program
		glAttachShader(shaderProgram.shaderID, shader);

		glLinkProgram(shaderProgram.shaderID);
		/*if (!CheckLinkStatus(shaderProgram.shaderID)) {
			shaderProgram.shaderID = -1;
			return false;
		}*/

		// Detach? and delete the shader object
		//glDetachShader(shaderProgram.shaderID, shader);
		glDeleteShader(shader);

		COMPHILOG_CORE_INFO("Shader ID Compile: %i ... success!", shaderProgram.shaderID);
		return true;
	}

	bool OpenGLShaderWizard::CheckCompileStatus(uint shaderID)
	{
		return checkStatus(shaderID, glGetProgramiv, glGetProgramInfoLog, GL_COMPILE_STATUS);
	}

	bool OpenGLShaderWizard::CheckLinkStatus(uint programID)
	{
		return checkStatus(programID, glGetShaderiv, glGetShaderInfoLog, GL_LINK_STATUS);
	}

	bool OpenGLShaderWizard::checkStatus(GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType)
	{
		GLint Status;
		objectPropertyGetterFunc(objectID, statusType, &Status);
		if (Status != GL_TRUE) {

			GLint infoLogLength;
			objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = new GLchar[infoLogLength];

			GLsizei bufferSize;
			getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
			COMPHILOG_CORE_ERROR("%s", buffer);
			delete[] buffer;
			return false;
		}
		return true;
	}

}

