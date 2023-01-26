#pragma once
#include "Comphi/Renderer/ITexture.h"
#include "ImageBufer.h"

namespace Comphi::Vulkan {

	class ImageView : public ITexture
	{
	public:
		void initTextureImageView(IFileRef& fileref, ImageBufferSpecification bufferSpecs = {});
		void initDepthImageView(VkExtent2D& swapChainImageBufferExtent);
		static void initSwapchainImageViews(VkSwapchainKHR swapchain, VkFormat SwapchainImageFormat, std::vector<ImageView>& swapchainImageViews);

		void cleanUp();
		VkImageView imageView;
		VkSampler textureSampler;
		ImageBuffer imageBuffer;

		ImageView() = default;

	protected:
		void allocateTextureSampler();
		void allocateImageView();
		bool isDepthImageView = false;
		bool hasTextureSampler = false;
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		
	};

}


