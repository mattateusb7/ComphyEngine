#include "cphipch.h"
#include "ShaderBinding.h"

namespace Comphi {

	void ShaderBinding::bindTextures(ShaderTextures& textures, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq)
	{
		TextureBinding textureBinding = {
			setID, descriptorID,
			textures
		};
		textureBindings[updateFreq].push_back(textureBinding);
	}

	void ShaderBinding::bindBuffers(ShaderBuffers& buffers, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq)
	{
		BufferBinding bufferBinding = {
			setID, descriptorID,
			buffers
		};
		bufferBindings[updateFreq].push_back(bufferBinding);
	}

	void ShaderBinding::bindTextures(TexturePtr& texture, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq)
	{
		auto textures = ShaderTextures();
		textures.push_back(texture);
		TextureBinding textureBinding = {
			setID, descriptorID,
			textures
		};
		textureBindings[updateFreq].push_back(textureBinding);
	}

	void ShaderBinding::bindBuffers(ShaderBufferDataPtr& bufferData, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq)
	{
		auto buffers = ShaderBuffers();
		buffers.push_back(bufferData);
		BufferBinding bufferBinding = {
			setID, descriptorID,
			buffers
		};
		bufferBindings[updateFreq].push_back(bufferBinding);

	}
}
