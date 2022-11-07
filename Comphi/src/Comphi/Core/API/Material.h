#pragma once

#include "Comphi/Renderer/Vulkan/Initialization/GraphicsPipeline.h"

namespace Comphi {
	class Material //TODO: public IMaterial API / make virtual implementations for different Materials ? or generic for all
	{
	public:
		struct InitializationData //TODO : may Move all shaders to graphics Pipeline?
		{
			std::vector<IShaderProgram*> shaders;
			std::vector<Vulkan::Texture*> textures;
		};
		Material(InitializationData properties);

		std::vector<Vulkan::Texture*>shaderTextures;
		std::vector<Vulkan::ShaderProgram*>shaderPrograms;
		std::shared_ptr<Vulkan::GraphicsPipeline> graphicsPipeline;

		~Material();
	protected:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		bool BindProgram(IShaderProgram& shaderProgram);
		bool UnbindProgram(IShaderProgram& shaderProgram);

	};
}

