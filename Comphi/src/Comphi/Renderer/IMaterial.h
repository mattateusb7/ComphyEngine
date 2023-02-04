#pragma once
#include "Comphi/Allocation/IObject.h"
#include "ITexture.h"
#include "IShaderProgram.h"

namespace Comphi {

	//API Abstraction
	typedef std::vector<ITexture*> ShaderTextures;
	typedef std::vector<IShaderProgram*> ShaderPrograms;

	struct MaterialResources
	{

		GraphicsPipelineConfiguration pipelineConfiguration;

		//TODO: DescriptorSets containing references to commandBuffer resources (Shader accessible data)
		// Descriptor Sets : https://youtu.be/5VBVWCg7riQ?t=117

		ShaderPrograms shaderPrograms = ShaderPrograms();
		ShaderTextures shaderTextures = ShaderTextures();
		//uniform MVPMatrix 
		//Uniforms
		//All kinds of data

	
	};

	class IMaterial : public IObject //TODO: public IMaterial API / make virtual implementations for different Materials ? or generic for all
	{
	};
}