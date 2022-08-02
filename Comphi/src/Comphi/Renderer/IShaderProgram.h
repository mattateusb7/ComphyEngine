#pragma once
#include "Comphi/Core/Core.h"
#include "Comphi/Platform/IFileRef.h"

namespace Comphi{

	enum class ShaderType
	{
		VertexShader,
		TessellationShader,
		GeometryShader,
		FragmentShader,
		ComputeShader,
	};

	class IShaderProgram
	{
	public:
		IShaderProgram(ShaderType shaderType, IFileRef& file) : m_shaderType(shaderType), shaderFile(file) {};
		virtual uint GetType() = 0; 
		//virtual uint GetTypeMask() = 0; 

		IFileRef& shaderFile;
		uint shaderID = -1;

	protected:
		ShaderType m_shaderType;
	};

}
