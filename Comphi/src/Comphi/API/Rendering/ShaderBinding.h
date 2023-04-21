#pragma once
#include "Comphi/API/Rendering/ShaderBufferData.h"
#include "Comphi/API/Rendering/TextureObject.h"
#include "Comphi/API/Rendering/Material.h"

namespace Comphi {

	struct ShaderBindingIds {
		uint layoutSetID;
		uint descriptorID;
	};

	struct TextureBinding : ShaderBindingIds {
		std::vector<TexturePtr>& textures;
	};
	
	struct BufferBinding : ShaderBindingIds {
		std::vector<BufferDataPtr>& buffers;
	};

	//struct TextureBinding : ShaderBindingIds {
	//	std::vector<TexturePtr>& textures;
	//};
	//
	//struct BufferBinding : ShaderBindingIds {
	//	std::vector<BufferDataPtr>& buffers;
	//};

	class ShaderBinding
	{
	public:
		ShaderBinding() = default;
		void bindTextures(std::vector<TexturePtr>& textures, LayoutSetUpdateFrequency setID, uint descriptorID);
		void bindBuffers(std::vector<BufferDataPtr>& buffers, LayoutSetUpdateFrequency setID, uint descriptorID);
		
		void bindTexture(TexturePtr& texture, LayoutSetUpdateFrequency setID, uint descriptorID);
		void bindBuffer(BufferDataPtr& bufferData, LayoutSetUpdateFrequency setID, uint descriptorID);

		std::map<LayoutSetUpdateFrequency, std::vector<TextureBinding>> textureBindings;
		std::map<LayoutSetUpdateFrequency, std::vector<BufferBinding>> bufferBindings;
	};
}


