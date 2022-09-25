#pragma once
#include "../GraphicsHandler.h"
#include "ImageBufer.h"

namespace Comphi::Vulkan {

	class ImageView : public ImageBuffer
	{
	public:
		ImageView() = default;
		ImageView(std::string filepath, const std::shared_ptr<GraphicsHandler>& graphicsHandler,
			VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

		void createImageView(ImageBuffer& imageBuffer, const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
		//NonAlocated ImageBuffer
		void createSwapchainImageView(VkImage& imageBufferObj, VkFormat& imageFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler);

		void cleanUp();
		VkImageView imageViewObj;
		std::shared_ptr<GraphicsHandler> graphicsHandler;

	protected:
		
	};

}


