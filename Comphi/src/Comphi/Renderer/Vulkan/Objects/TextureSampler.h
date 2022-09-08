#pragma once
#include "ImageView.h"

namespace Comphi::Vulkan {

	class TextureSampler
	{
	public:
		TextureSampler(const ImageView& textureImageView, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		~TextureSampler();

		VkImageView getVkImageView() { return textureImageView.get()->imageViewObj; };
		VkSampler getVkSampler() { return textureSampler; }
		void cleanUp();

	protected:
		VkSampler textureSampler;
		std::shared_ptr<ImageView> textureImageView;
		std::shared_ptr<GraphicsHandler> graphicsHandler;
	};

}