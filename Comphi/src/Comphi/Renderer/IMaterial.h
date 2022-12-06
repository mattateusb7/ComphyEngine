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

	class IMaterial : public MaterialProperties //TODO: public IMaterial API / make virtual implementations for different Materials ? or generic for all
	{
	public:
		virtual void sendDescriptorSet(std::vector<IUniformBuffer> MVP_ubos) = 0;
	};
}