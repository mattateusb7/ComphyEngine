#pragma once
#include "../GraphicsHandler.h"
#include "ImageBufer.h"

namespace Comphi::Vulkan {

	class ImageView
	{
	public:
		ImageView(VkImage& imgBuffer, VkFormat imgFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		ImageView(ImageBuffer& imgBuffer);
		ImageView() = default;
		void createImageView(VkImage& imgBuffer, VkFormat imgFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler);

		void cleanUp();

		VkImageView imageViewObj;
		std::shared_ptr<GraphicsHandler> graphicsHandler;

	protected:
		
	};

}


