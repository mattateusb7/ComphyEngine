#pragma once
#include <glad\glad.h>
#include "../ShaderPipeline.h"

namespace Comphi {

	enum class OpenGLShaderType {
		VertexShader = GL_VERTEX_SHADER,
		//TessellationShader = GL_TESS_EVALUATION_SHADER,
		GeometryShader = GL_GEOMETRY_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
		ComputeShader = GL_COMPUTE_SHADER
	};
	enum class OpenGLShaderTypeMask {
		VertexShader = GL_VERTEX_SHADER_BIT,
		//TessellationShader = GL_TESS_EVALUATION_SHADER_BIT,
		GeometryShader = GL_GEOMETRY_SHADER_BIT,
		FragmentShader = GL_FRAGMENT_SHADER_BIT,
		ComputeShader = GL_COMPUTE_SHADER_BIT
	};
	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
		OpenGLShaderProgram(ShaderType shaderType, FileRef* shaderFile) : ShaderProgram(shaderType, shaderFile) {};
		~OpenGLShaderProgram();
		uint GetType() override;
		uint GetTypeMask() override;
	};

}