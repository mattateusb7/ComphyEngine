#pragma once
#include "ImageBufer.h"

namespace Comphi::Vulkan {

	class ImageView : public ImageBuffer
	{
	public:
		//default constructer auto alocates ImageBuffer
		ImageView() = default;
		ImageView(std::string filepath, ImgSpecification specification = ImgSpecification());
		VkSampler initTextureSampler();

		//from ImageBuffer: 
		//<< VkImage bufferObj; 
		//<< bufferMemory;
		//<< bufferSize;
		//<< uint32_t width;
		//<< uint32_t height;
		//<< VkFormat imageFormat;
		//<< VkImageLayout imageLayout;

		//External ImageBuffer
		void initSwapchainImageView(VkImage& imageBufferObj, VkFormat& imageFormat);
		void initDepthImageView(ImageBuffer& swapChainImageBuffer);

		void cleanUp();
		VkImageView imageViewObj;
		VkImageAspectFlags aspectFlags;
		VkSampler textureSamplerObj;

	protected:
		void initImageView();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		
	};

	typedef ImageView Texture;

}


