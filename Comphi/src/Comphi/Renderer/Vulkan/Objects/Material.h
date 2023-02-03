#pragma once

#include "Comphi/Allocation/IObject.h"
#include "Comphi/Renderer/IMaterial.h"
#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "GraphicsPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	//API Abstraction
	typedef std::vector<ITexture*> ShaderTextures;
	typedef std::vector<IShaderProgram*> ShaderPrograms;
	GraphicsPipelineConfiguration pipelineConfiguration;

	struct MaterialResources
	{
		IGraphicsPipeline* graphicsPipeline;
		ShaderPrograms shaderPrograms = ShaderPrograms();
		ShaderTextures shaderTextures = ShaderTextures();

		//TODO: DescriptorSets Specifications derived from data
		// containing references to commandBuffer resources (Shader accessible data)
		// Descriptor Sets : https://youtu.be/5VBVWCg7riQ?t=117

	
		//uniform MVPMatrix 
		//Uniforms
		//All kinds of data
		
		//Binding IDS ?
		//Binding points ID do not interfeer with eachother, each has their own IDs: Graphics, Compute, Ray_tracing (vkPipelineBindPoint)
	};

	class Material : public IMaterial
	{
	public:

		Material(MaterialResources& properties);
		virtual void cleanUp() override;

		MaterialResources properties;

	protected:

		bool bindShaderProgram(IShaderProgram& shaderProgram);
		bool unbindShaderProgram(IShaderProgram& shaderProgram);
		
	};

}

