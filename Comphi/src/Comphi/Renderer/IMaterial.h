#pragma once
#include "Comphi/Allocation/IObject.h"
#include "ITexture.h"
#include "IShaderProgram.h"

namespace Comphi {

	typedef std::vector<ITexture*> ShaderTextures;
	typedef std::vector<IShaderProgram*> ShaderPrograms;
	
	//Shader Resources
	struct MaterialProperties
	{
		//linked MaterialProperties:
		ShaderPrograms shaderPrograms = ShaderPrograms();
		ShaderTextures shaderTextures = ShaderTextures();
	};

	class IMaterial : public IObject, public MaterialProperties //TODO: public IMaterial API / make virtual implementations for different Materials ? or generic for all
	{
		//Default Material
	};
}