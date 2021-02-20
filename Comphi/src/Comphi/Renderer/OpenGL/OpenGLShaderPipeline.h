#pragma once
#include <glad\glad.h>
#include "../ShaderPipeline.h"

namespace Comphi{

	class OpenGLShaderPipeline : public ShaderPipeline
	{
	public:
		OpenGLShaderPipeline();
		~OpenGLShaderPipeline();
		bool InitPipeline() override;
		bool BindProgram(ShaderProgram& shaderProgram) override;
		bool UnbindProgram(ShaderProgram& shaderProgram) override;

		/*bool LinkProgram(ShaderProgram& shaderProgram)
		{
			// Link the program.
			glLinkProgram(shaderProgram.ID);

			GLint isLinked = 0;
			glGetProgramiv(shaderProgram.ID, GL_LINK_STATUS, &isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(shaderProgram.ID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(shaderProgram.ID, maxLength, &maxLength, &infoLog[0]);

				// The program is useless now. So delete it.
				glDeleteProgram(shaderProgram.ID);

				// Provide the infolog in whatever manor you deem best.
				COMPHILOG_CORE_INFO("Shader Link Error : %s", infoLog.data());
				// Exit with failure.
				return false;

			}
		}*/

	};
	
}