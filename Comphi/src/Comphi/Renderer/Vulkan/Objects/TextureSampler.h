#pragma once
#include "ImageView.h"

namespace Comphi::Vulkan {

	class TextureSampler
	{
	public:
		TextureSampler(const std::shared_ptr<ImageView>& textureImageView, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		~TextureSampler();

		VkSampler textureSampler;
		std::shared_ptr<ImageView> textureImageView;
		std::shared_ptr<GraphicsHandler> graphicsHandler;
		void cleanUp();

	protected:

	};

}