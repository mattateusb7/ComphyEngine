#pragma once
#include "Comphi/API/Rendering/ShaderBufferData.h"
#include "Comphi/API/Rendering/TextureObject.h"
#include "Comphi/API/Rendering/ShaderObject.h"
#include "Comphi/API/Rendering/Material.h"

namespace Comphi {
	
	struct ShaderResources {
		ShaderTextures textures;
		ShaderBuffers buffers;
	};

	class MaterialInstance : public IObject
	{
	public:
		MaterialInstance(MaterialPtr& parent) :
		parent(parent) 
		{}

		MaterialPtr& parent;
		ShaderResources resources;

		virtual void cleanUp() override {};
		~MaterialInstance() = default;
	};
	
	typedef std::shared_ptr<MaterialInstance> MaterialInstancePtr;

}