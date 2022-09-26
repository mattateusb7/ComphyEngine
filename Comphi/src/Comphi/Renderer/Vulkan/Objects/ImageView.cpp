#include "cphipch.h"
#include "ImageView.h"

namespace Comphi::Vulkan {

	ImageView::ImageView(std::string filepath, const std::shared_ptr<GraphicsHandler>& graphicsHandler, VkFormat format, VkImageAspectFlags aspectFlags, VkImageTiling tiling, VkImageUsageFlags usage)
		: ImageBuffer(filepath, graphicsHandler, format, tiling, usage)
	{
		this->graphicsHandler = graphicsHandler;
		this->aspectFlags = aspectFlags;
		initImageView();
	}

	void ImageView::initSwapchainImageView(VkImage& imageBufferObj, VkFormat& imageFormat, const std::shared_ptr<GraphicsHandler>& graphicsHandler)
	{
		this->graphicsHandler = graphicsHandler;
		this->bufferObj = imageBufferObj;
		this->imageFormat = imageFormat;
		this->aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		initImageView();
	}

	void ImageView::initDepthImageView(ImageBuffer& swapChainImageBuffer)
	{
		this->graphicsHandler = swapChainImageBuffer.graphicsHandler;
		initDepthImageBuffer(swapChainImageBuffer, findDepthFormat());
		this->aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
		initImageView();
	}

	void ImageView::initImageView()
	{

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = bufferObj;

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; //1D textures, 2D textures, 3D textures and cube maps.
		createInfo.format = imageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping

		createInfo.subresourceRange.aspectMask = aspectFlags;
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

	VkFormat ImageView::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(*graphicsHandler->physicalDevice.get(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}

	VkFormat ImageView::findDepthFormat() {
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	void ImageView::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy ImageView");
		vkDestroyImageView(*graphicsHandler->logicalDevice.get(), imageViewObj, nullptr);
	}

}