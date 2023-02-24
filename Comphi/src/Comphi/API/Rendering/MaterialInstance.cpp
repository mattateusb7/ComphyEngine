#include "cphipch.h"
#include "MaterialInstance.h"

namespace Comphi {
	MaterialInstance::MaterialInstance(MaterialPtr& parent) : parent(parent) {}

	void MaterialInstance::linkTexture(TexturePtr texture)
	{
		resources.textures.push_back(texture);
	}

	void MaterialInstance::linkBuffer(ShaderBufferDataPtr bufferData)
	{
		resources.buffers.push_back(bufferData);
	}


}
