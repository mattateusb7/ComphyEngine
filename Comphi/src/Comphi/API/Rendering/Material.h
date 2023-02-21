#pragma once

#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "Comphi/API/Rendering/ShaderObject.h"
#include "Comphi/API/Rendering/MaterialInstance.h"


namespace Comphi {

	class Material : IGraphicsPipeline
	{
	public:
		Material() = default;
		ShaderPrograms shaders;

		~Material() = default;
	};

	typedef std::shared_ptr<Material> MaterialPtr;

}