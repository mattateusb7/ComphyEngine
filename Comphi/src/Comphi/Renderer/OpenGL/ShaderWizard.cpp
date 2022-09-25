#include "cphipch.h"
#include "ShaderWizard.h"
#include "GLError.h"

namespace Comphi::OpenGL {
	bool ShaderWizard::Compile(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram);

		if (_shaderProgram->shaderID == -1) return false;

		//glCreateShaderProgramv ?auto setup?
		_shaderProgram->shaderID = glCreateProgram();

		// Declare that program is separable
		glProgramParameteri(_shaderProgram->shaderID, GL_PROGRAM_SEPARABLE, GL_TRUE);

		GLuint shader = glCreateShader(shaderProgram.GetType());

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)shaderProgram.shaderFile.getData().c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);
		if (!CheckCompileStatus(shader)) {
			glDetachShader(_shaderProgram->shaderID, shader);
			glDeleteShader(shader);
			_shaderProgram->shaderID = -1;
			return false;
		}
		// Shader compilation is successful.

		// Attach the shader to its respective program
		glAttachShader(_shaderProgram->shaderID, shader);

		glLinkProgram(_shaderProgram->shaderID);
		if (!CheckLinkStatus(_shaderProgram->shaderID)) {
			glDetachShader(_shaderProgram->shaderID, shader);
			glDeleteShader(shader);
			_shaderProgram->shaderID = -1;
			return false;
		}

		COMPHILOG_CORE_INFO("Successfully Attached Shader \"" + shaderProgram.shaderFile.getFilename() + "\" to ID: " + std::to_string(_shaderProgram->shaderID));
		
		/*debug Content*/
		//COMPHILOG_CORE_INFO(shaderProgram.shaderFile.getFileContent());

		// Detach? and delete the shader object
		glDeleteShader(shader);
		check_gl_error();
		return true;
	}

	bool ShaderWizard::CheckCompileStatus(uint shaderID)
	{
		return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
	}

	bool ShaderWizard::CheckLinkStatus(uint programID)
	{
		return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
	}

	bool ShaderWizard::checkStatus(GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType)
	{
		GLint Status;
		objectPropertyGetterFunc(objectID, statusType, &Status);
		if (Status != GL_TRUE) {

			GLint infoLogLength;
			objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::unique_ptr<GLchar[]> buffer = std::make_unique<GLchar[]>(infoLogLength);

			GLsizei bufferSize;
			getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer.get());
			COMPHILOG_CORE_ERROR(buffer);
			return false;
		}
		return true;
	}

}

