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

	enum ShaderStageFlag {
		FragmentStage = 0x00000010,//VK_SHADER_STAGE_FRAGMENT_BIT
		VertexStage = 0x00000001, //VK_SHADER_STAGE_VERTEX_BIT
		AllGraphics = 0x0000001F,
		AllStages = 0x7FFFFFFF
	};

	/*
	VK_SHADER_STAGE_VERTEX_BIT = 0x00000001,
    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT = 0x00000002,
    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT = 0x00000004,
    VK_SHADER_STAGE_GEOMETRY_BIT = 0x00000008,
    VK_SHADER_STAGE_FRAGMENT_BIT = 0x00000010,
    VK_SHADER_STAGE_COMPUTE_BIT = 0x00000020,
    VK_SHADER_STAGE_ALL_GRAPHICS = 0x0000001F,
    VK_SHADER_STAGE_ALL = 0x7FFFFFFF,
	*/

	class IShaderProgram : public IObject
	{
	public:
		IShaderProgram(ShaderType shaderType, IFileRef& file) : m_shaderType(shaderType), shaderFile(file) {};
		virtual const uint GetType(){ return (uint)m_shaderType; };
		IFileRef& shaderFile;
		std::string entryPointFunctionName = "main";
	protected:
		ShaderType m_shaderType;
	};

}
