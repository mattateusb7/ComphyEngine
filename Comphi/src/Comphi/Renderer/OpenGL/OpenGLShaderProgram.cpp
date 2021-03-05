#include "cphipch.h"
#include "OpenGLShaderProgram.h"

namespace Comphi::OpenGL{
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(shaderID);
	}
	uint ShaderProgram::GetType()
{
		switch (this->m_shaderType)
		{
		case Comphi::ShaderType::VertexShader:
			return (int)ShaderType::VertexShader;
		case Comphi::ShaderType::GeometryShader:
			return (int)ShaderType::GeometryShader;
		case Comphi::ShaderType::FragmentShader:
			return (int)ShaderType::FragmentShader;
		case Comphi::ShaderType::ComputeShader:
			return (int)ShaderType::ComputeShader;
		default:
			return -1;
		}
	}

	uint ShaderProgram::GetTypeMask()
	{
		switch (this->m_shaderType)
		{
		case Comphi::ShaderType::VertexShader:
			return (int)ShaderTypeMask::VertexShader;
		case Comphi::ShaderType::GeometryShader:
			return (int)ShaderTypeMask::GeometryShader;
		case Comphi::ShaderType::FragmentShader:
			return (int)ShaderTypeMask::FragmentShader;
		case Comphi::ShaderType::ComputeShader:
			return (int)ShaderTypeMask::ComputeShader;
		default:
			return -1;
		}
	}

}
