#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class ImageView
	{
	public:
		ImageView(VkImage& imgBuffer, VkFormat imgFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		ImageView() = default;
		void createImageView(VkImage& imgBuffer, VkFormat imgFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler);

		void cleanUp();

		VkImageView imageViewObj;
		std::shared_ptr<GraphicsHandler> graphicsHandler;

	protected:
		
	};

}


