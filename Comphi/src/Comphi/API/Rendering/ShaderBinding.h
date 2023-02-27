#pragma once
#include "Comphi/API/Rendering/ShaderBufferData.h"
#include "Comphi/API/Rendering/TextureObject.h"
#include "Comphi/API/Rendering/Material.h"

namespace Comphi {

	struct ShaderBindingIds {
		uint setID;
		uint descriptorID;
	};

	struct TextureBinding : ShaderBindingIds {
		ShaderTextures& textures;
	};

	struct BufferBinding : ShaderBindingIds {
		ShaderBuffers& buffers;
	};

	class ShaderBinding
	{
	public:
		ShaderBinding() = default;
		void bindTextures(ShaderTextures& textures, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq = LayoutSetUpdateFrequency::PerMaterialInstance);
		void bindBuffers(ShaderBuffers& buffers, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq = LayoutSetUpdateFrequency::PerMaterialInstance);
		
		void bindTextures(TexturePtr& texture, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq = LayoutSetUpdateFrequency::PerMaterialInstance);
		void bindBuffers(ShaderBufferDataPtr& bufferData, uint setID, uint descriptorID, LayoutSetUpdateFrequency updateFreq = LayoutSetUpdateFrequency::PerMaterialInstance);

		std::map<LayoutSetUpdateFrequency, std::vector<TextureBinding>> textureBindings;
		std::map<LayoutSetUpdateFrequency, std::vector<BufferBinding>> bufferBindings;
	};
}


