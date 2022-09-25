#include "cphipch.h"
#include "ImageView.h"

namespace Comphi::Vulkan {

	ImageView::ImageView(std::string filepath, const std::shared_ptr<GraphicsHandler>& graphicsHandler, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage) 
		: ImageBuffer(filepath, graphicsHandler, format, tiling, usage)
	{
		createImageView(*this,graphicsHandler);
	}

	void ImageView::createSwapchainImageView(VkImage& imageBufferObj, VkFormat& imageFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler)
	{
		this->bufferObj = imageBufferObj;
		this->imageFormat = imageFormat;
		createImageView(*this, graphicsHandler);
	}

	void ImageView::createImageView(ImageBuffer& imageBuffer, const std::shared_ptr<GraphicsHandler>& graphicsHandler)
	{
		this->graphicsHandler = graphicsHandler;

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = imageBuffer.bufferObj;

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; //1D textures, 2D textures, 3D textures and cube maps.
		createInfo.format = imageBuffer.imageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		//If you were working on a stereographic 3D application, then you would create a swap chain with multiple layers. 
		//You could then create multiple image views for each image 
		//representing the views for the left and right eyes by accessing different layers.

		if (vkCreateImageView(*graphicsHandler->logicalDevice.get(), &createInfo, nullptr, &imageViewObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create image view!");
			throw std::runtime_error("failed to create image view!");
			return;
		}
		COMPHILOG_CORE_INFO("created image view! successfully!");
	}

	void ImageView::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy ImageView");
		vkDestroyImageView(*graphicsHandler->logicalDevice.get(), imageViewObj, nullptr);
	}

}