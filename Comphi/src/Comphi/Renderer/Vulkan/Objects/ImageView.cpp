#include "cphipch.h"
#include "ImageView.h"

namespace Comphi::Vulkan {

	ImageView::ImageView(std::string filepath, ImgSpecification spec)
		: ImageBuffer(filepath, spec)
	{
		this->aspectFlags = spec.aspectFlags;
		initImageView();
		initTextureSampler();
	}

	VkSampler ImageView::initTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;

		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(*GraphicsHandler::get()->physicalDevice.get(), &properties);

		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

		samplerInfo.unnormalizedCoordinates = VK_FALSE;//[0..1]UVW

		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(*GraphicsHandler::get()->logicalDevice.get(), &samplerInfo, nullptr, &textureSamplerObj) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
		COMPHILOG_CORE_INFO("Created TextureSampler successfully!");
	}

	void ImageView::initSwapchainImageView(VkImage& imageBufferObj, VkFormat& imageFormat)
	{
		this->bufferObj = imageBufferObj;
		this->imageFormat = imageFormat;
		this->aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		initImageView();
	}

	void ImageView::initDepthImageView(ImageBuffer& swapChainImageBuffer)
	{
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

		if (vkCreateImageView(*GraphicsHandler::get()->logicalDevice.get(), &createInfo, nullptr, &imageViewObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create image view!");
			throw std::runtime_error("failed to create image view!");
			return;
		}
		COMPHILOG_CORE_INFO("created image view! successfully!");
	}

	VkFormat ImageView::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(*GraphicsHandler::get()->physicalDevice.get(), format, &props);

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
		vkDestroyImageView(*GraphicsHandler::get()->logicalDevice.get(), imageViewObj, nullptr);
		
		COMPHILOG_CORE_INFO("vkDestroy Destroy textureSampler");
		vkDestroySampler(*GraphicsHandler::get()->logicalDevice.get(), textureSamplerObj, nullptr);

	}

}