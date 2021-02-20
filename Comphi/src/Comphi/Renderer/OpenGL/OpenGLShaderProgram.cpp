#include "cphipch.h"
#include "OpenGLShaderProgram.h"

namespace Comphi{
	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		glDeleteProgram(shaderID);
	}
	uint OpenGLShaderProgram::GetType()
{
		switch (this->m_shaderType)
		{
		case ShaderType::VertexShader:
			return (int)OpenGLShaderType::VertexShader;
		case ShaderType::GeometryShader:
			return (int)OpenGLShaderType::GeometryShader;
		case ShaderType::FragmentShader:
			return (int)OpenGLShaderType::FragmentShader;
		case ShaderType::ComputeShader:
			return (int)OpenGLShaderType::ComputeShader;
		default:
			return -1;
		}
	}

	uint OpenGLShaderProgram::GetTypeMask()
	{
		switch (this->m_shaderType)
		{
		case ShaderType::VertexShader:
			return (int)OpenGLShaderTypeMask::VertexShader;
		case ShaderType::GeometryShader:
			return (int)OpenGLShaderTypeMask::GeometryShader;
		case ShaderType::FragmentShader:
			return (int)OpenGLShaderTypeMask::FragmentShader;
		case ShaderType::ComputeShader:
			return (int)OpenGLShaderTypeMask::ComputeShader;
		default:
			return -1;
		}
	}
}
