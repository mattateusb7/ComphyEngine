#include "cphipch.h"
#include "ShaderBinding.h"

namespace Comphi {

	void ShaderBinding::bindTextures(std::vector<TexturePtr>& textures, LayoutSetUpdateFrequency layoutSetID, uint descriptorID)
	{
		//std::vector<ITexture*>
		//for (auto const& tex : textures) {
		//
		//}

		TextureBinding textureBinding = {
			layoutSetID, descriptorID,
			textures
		};
		textureBindings[layoutSetID].push_back(textureBinding);
	}

	void ShaderBinding::bindBuffers(std::vector<BufferDataPtr>& buffers, LayoutSetUpdateFrequency layoutSetID, uint descriptorID)
	{
		BufferBinding bufferBinding = {
			layoutSetID, descriptorID,
			buffers
		};
		bufferBindings[layoutSetID].push_back(bufferBinding);
	}

	void ShaderBinding::bindTexture(TexturePtr& texture, LayoutSetUpdateFrequency layoutSetID, uint descriptorID)
	{
		auto textures = std::vector<TexturePtr>();
		textures.push_back(texture);
		TextureBinding textureBinding = {
			layoutSetID, descriptorID,
			textures
		};
		textureBindings[layoutSetID].push_back(textureBinding);
	}

	void ShaderBinding::bindBuffer(BufferDataPtr& bufferData, LayoutSetUpdateFrequency layoutSetID, uint descriptorID)
	{
		auto buffers = std::vector<BufferDataPtr>();
		buffers.push_back(bufferData);
		BufferBinding bufferBinding = {
			layoutSetID, descriptorID,
			buffers
		};
		bufferBindings[layoutSetID].push_back(bufferBinding);

	}
}
