#pragma once
#include "Comphi/API/Rendering/ShaderBufferData.h"
#include "Comphi/API/Rendering/TextureObject.h"
#include "Comphi/API/Rendering/Material.h"

namespace Comphi {
	
	struct ShaderResources {
		ShaderTextures textures;
		ShaderBuffers buffers;
	};

	class MaterialInstance : public IObject
	{
	public:
		MaterialInstance(MaterialPtr& parent);

		void linkTexture(TexturePtr texture);
		void linkBuffer(ShaderBufferDataPtr bufferData);
		
		MaterialPtr& parent;
		ShaderResources resources;

		virtual void cleanUp() override {};
	};
	
	typedef std::shared_ptr<MaterialInstance> MaterialInstancePtr;

}