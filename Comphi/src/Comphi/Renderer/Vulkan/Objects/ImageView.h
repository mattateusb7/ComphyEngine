#pragma once
#include "Comphi/Renderer/ITexture.h"
#include "ImageBufer.h"

namespace Comphi::Vulkan {

	class ImageView : public ITexture
	{
	public:
		//factory
		static ImageView createTextureImageView(IFileRef& fileref, ImageBufferSpecification bufferSpecs = {});
		static std::vector<ImageView> createSwapchainImageViews(VkSwapchainKHR swapchain, VkFormat SwapchainImageFormat);
		static ImageView createDepthImageView(VkExtent2D& swapChainImageBufferExtent);

		void cleanUp();
		VkImageView imageView;
		VkSampler textureSampler;
		ImageBuffer imageBuffer;

		ImageView() = default;
		void allocateImageView();
		void allocateTextureSampler();

	protected:
		bool isSwapchainImage;
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		
	};

}


