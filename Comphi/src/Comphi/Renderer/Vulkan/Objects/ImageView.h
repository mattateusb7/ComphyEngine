#pragma once
#include "../GraphicsHandler.h"
#include "ImageBufer.h"

namespace Comphi::Vulkan {

	class ImageView : public ImageBuffer
	{
	public:
		//default constructer auto alocates ImageBuffer
		ImageView() = default;
		ImageView(std::string filepath, const std::shared_ptr<GraphicsHandler>& graphicsHandler,
			VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

		//from ImageBuffer: 
		//<< VkImage bufferObj; 
		//<< bufferMemory;
		//<< bufferSize;
		//<< uint32_t width;
		//<< uint32_t height;
		//<< graphicsHandler;
		//<< VkFormat imageFormat;
		//<< VkImageLayout imageLayout;

		void initDepthImageView(ImageBuffer& swapChainImageBuffer);

		//NonAlocated ImageBuffer
		void initSwapchainImageView(VkImage& imageBufferObj, VkFormat& imageFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler);

		void cleanUp();
		VkImageView imageViewObj;
		VkImageAspectFlags aspectFlags;

	protected:
		void initImageView();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		
	};

}


