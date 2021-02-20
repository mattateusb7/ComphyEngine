#pragma once
#include "Comphi/Platform/FileManager.h"
#include <Comphi\Platform\Windows\FileRef.h>

namespace Comphi{

	enum class ShaderType
	{
		VertexShader = 0,
		//TessellationShader = 1,
		GeometryShader = 2,
		FragmentShader = 3,
		ComputeShader = 4,
	};

	class ShaderProgram
	{
	public:
		ShaderProgram(ShaderType shaderType, FileManager& file) : m_shaderType(shaderType), shaderFile(file){
			shaderFile.getData(shaderSource);
		};
		virtual ~ShaderProgram() = default;

		virtual uint GetType() = 0; 
		virtual uint GetTypeMask() = 0; 
		
		FileManager& shaderFile;
		std::string shaderSource;
		uint shaderID;

	protected:
		ShaderType m_shaderType;
	};

}
