#pragma once
#include "Comphi/Platform/IFileRef.h"

namespace Comphi{

	enum class ShaderType
	{
		VertexShader = 0,
		//TessellationShader = 1,
		GeometryShader = 2,
		FragmentShader = 3,
		ComputeShader = 4,
	};

	class IShaderProgram
	{
	public:
		IShaderProgram(ShaderType shaderType, IFileRef* file) : m_shaderType(shaderType), shaderFile(*file) {};
		virtual ~IShaderProgram() = default;

		virtual uint GetType() = 0; 
		virtual uint GetTypeMask() = 0; 

		IFileRef& shaderFile;
		uint shaderID;

	protected:
		ShaderType m_shaderType;
	};

}
