#pragma once

#include "Comphi/Renderer/Vulkan/Initialization/GraphicsPipeline.h"

namespace Comphi {

	struct MaterialProperties
	{
		std::vector<IShaderProgram*> shaders;
		std::vector<Vulkan::Texture*> textures;
		Vulkan::DescriptorPool* descriptorPool;
	};

	class Material //TODO: public IMaterial API / make virtual implementations for different Materials ? or generic for all
	{
	public:
		
		Material(MaterialProperties properties);

		//linked MaterialProperties:
		std::vector<Vulkan::Texture*>shaderTextures;
		std::vector<Vulkan::ShaderProgram*>shaderPrograms;
		
		//Each Material owns one Shader/Graphics Pipeline
		Vulkan::GraphicsPipeline graphicsPipeline;

		~Material();
	protected:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		bool BindProgram(IShaderProgram& shaderProgram);
		bool UnbindProgram(IShaderProgram& shaderProgram);

	};
	#define MaterialInstance std::shared_ptr<Material>
	#define MakeMaterialInstance std::make_shared<Material>
}

