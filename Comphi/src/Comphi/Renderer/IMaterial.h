#pragma once
#include "Comphi/Allocation/IObject.h"
#include "ITexture.h"
#include "IShaderProgram.h"
#include "IGraphicsPipeline.h"

namespace Comphi {

	//API Abstraction
	typedef std::vector<ITexture*> ShaderTextures;
	typedef std::vector<IShaderProgram*> ShaderPrograms;

	struct MaterialResources
	{
		IGraphicsPipeline* graphicsPipeline;
	};

	class IMaterial : public IObject, public IGraphicsPipeline
	{};

}