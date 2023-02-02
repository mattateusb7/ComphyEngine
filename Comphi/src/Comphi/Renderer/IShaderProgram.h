#pragma once
#include "Comphi/Allocation/IObject.h"
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

	class IShaderProgram : public IObject
	{
	public:
		IShaderProgram(ShaderType shaderType, IFileRef& file) : m_shaderType(shaderType), shaderFile(file) {};
		virtual const uint GetType(){ return (uint)m_shaderType; };
		IFileRef& shaderFile;

	protected:
		ShaderType m_shaderType;
	};

}
