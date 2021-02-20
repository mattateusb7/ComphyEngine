#include "cphipch.h"
#include "OpenGLShaderPipeline.h"

namespace Comphi {
	OpenGLShaderPipeline::OpenGLShaderPipeline()
	{
	
	}
	
	OpenGLShaderPipeline::~OpenGLShaderPipeline()
	{
		glUseProgramStages(m_pipeline, GL_ALL_SHADER_BITS, 0);
		glDeleteProgramPipelines(1, &m_pipeline);
	}
	
	bool OpenGLShaderPipeline::InitPipeline()
	{
		// Init program pipeline
		if (m_pipeline == -1)
		{
			glDeleteProgramPipelines(1, &m_pipeline);
			glGenProgramPipelines(1, &m_pipeline);
		}
		glUseProgramStages(m_pipeline, GL_ALL_SHADER_BITS, 0);

		return true;
	}

	bool OpenGLShaderPipeline::BindProgram(ShaderProgram& shaderProgram)
	{
		// Attach the first program to the vertex stage, and the second program
		// to the geometry and fragment stages
		glUseProgramStages(m_pipeline, shaderProgram.GetTypeMask(), shaderProgram.shaderID);
		return true;
	}

	bool OpenGLShaderPipeline::UnbindProgram(ShaderProgram& shaderProgram)
	{
		glUseProgramStages(m_pipeline, shaderProgram.GetTypeMask(), 0);
		return false;
	}

}
