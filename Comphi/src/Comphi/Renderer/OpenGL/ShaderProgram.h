#pragma once
#include <glad\glad.h>
#include "Comphi/Renderer/IShaderProgram.h"

namespace Comphi::OpenGL {

	enum class ShaderType {
		VertexShader = GL_VERTEX_SHADER,
		//TessellationShader = GL_TESS_EVALUATION_SHADER,
		GeometryShader = GL_GEOMETRY_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
		ComputeShader = GL_COMPUTE_SHADER
	};
	enum class ShaderTypeMask {
		VertexShader = GL_VERTEX_SHADER_BIT,
		//TessellationShader = GL_TESS_EVALUATION_SHADER_BIT,
		GeometryShader = GL_GEOMETRY_SHADER_BIT,
		FragmentShader = GL_FRAGMENT_SHADER_BIT,
		ComputeShader = GL_COMPUTE_SHADER_BIT
	};
	class ShaderProgram : public IShaderProgram
	{
	public:
		ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile);
		~ShaderProgram();
		const uint GetType() override;
		const uint GetTypeMask();

		uint shaderID = -1;
	};

}