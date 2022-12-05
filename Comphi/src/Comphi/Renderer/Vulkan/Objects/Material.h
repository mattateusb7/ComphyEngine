#pragma once

#include "Comphi/Renderer/IMaterial.h"
#include "GraphicsPipeline.h"
#include "ShaderProgram.h"

namespace Comphi::Vulkan {

	class Material : public IMaterial //TODO: public IMaterial API / make virtual implementations for different Materials ? or generic for all
	{
	public:
		
		Material(Comphi::MaterialProperties properties);
		~Material();

		//Each Material owns one Shader/Graphics Pipeline
		GraphicsPipeline graphicsPipeline;

		//linked MaterialProperties:
		std::vector<ShaderProgram*> shaderPrograms = std::vector<ShaderProgram*>();

	protected:

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		bool BindProgram(IShaderProgram& shaderProgram);
		bool UnbindProgram(IShaderProgram& shaderProgram);

		// Inherited via IMaterial
		virtual void sendDescriptorSet(std::vector<IUniformBuffer> MVP_ubos) override;
		virtual void bind(void* commandBuffer) override;
		virtual void bindDescriptorSet(void* commandBuffer, uint32_t currentFrame) override;

	};

}

