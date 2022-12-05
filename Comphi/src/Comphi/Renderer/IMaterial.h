#pragma once
#include "ITexture.h"
#include "IShaderProgram.h"
#include "IUniformBuffer.h"

namespace Comphi {

	typedef std::vector<ITexture*> ShaderTextures;
	typedef std::vector<IShaderProgram*> ShaderPrograms;
	struct MaterialProperties
	{
		//linked MaterialProperties:
		ShaderPrograms shaderPrograms = ShaderPrograms();
		ShaderTextures shaderTextures = ShaderTextures();
	};

	class IMaterial : public MaterialProperties
	{
	public:
		virtual void sendDescriptorSet(std::vector<IUniformBuffer> MVP_ubos) = 0;
		virtual void bind(void* commandBuffer) = 0;
		virtual void bindDescriptorSet(void* commandBuffer, uint32_t currentFrame) = 0;
	};
}