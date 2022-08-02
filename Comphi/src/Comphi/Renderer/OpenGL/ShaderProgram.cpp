#include "cphipch.h"
#include "ShaderProgram.h"
#include "ShaderWizard.h"

namespace Comphi::OpenGL{

	ShaderProgram::ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile) : IShaderProgram(shaderType, shaderFile) {
		ShaderWizard::CompileShader(*this);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(shaderID);
	}

	uint ShaderProgram::GetType()
	{
		switch (this->m_shaderType)
		{
		case Comphi::ShaderType::VertexShader:
			return (uint)ShaderType::VertexShader;
		case Comphi::ShaderType::GeometryShader:
			return (uint)ShaderType::GeometryShader;
		case Comphi::ShaderType::FragmentShader:
			return (uint)ShaderType::FragmentShader;
		case Comphi::ShaderType::ComputeShader:
			return (uint)ShaderType::ComputeShader;
		default:
			return -1;
		}
	}

	uint ShaderProgram::GetTypeMask()
	{
		switch (this->m_shaderType)
		{
		case Comphi::ShaderType::VertexShader:
			return (uint)ShaderTypeMask::VertexShader;
		case Comphi::ShaderType::GeometryShader:
			return (uint)ShaderTypeMask::GeometryShader;
		case Comphi::ShaderType::FragmentShader:
			return (uint)ShaderTypeMask::FragmentShader;
		case Comphi::ShaderType::ComputeShader:
			return (uint)ShaderTypeMask::ComputeShader;
		default:
			return -1;
		}
	}

}
