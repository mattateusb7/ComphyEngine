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

