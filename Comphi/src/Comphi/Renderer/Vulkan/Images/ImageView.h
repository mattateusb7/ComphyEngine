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

		virtual void cleanUp() override; //IObject

		VkImageView imageView;
		VkSampler textureSampler; 
		//TODO maybe separate sampler from texture 
		//and use it as a shader resource that can be reused for differernt textures
		ImageBuffer imageBuffer;
		//void*

		ImageView() = default;

	protected:
		void allocateTextureSampler();
		void allocateImageView();
		bool isSwapchainImage = false;
		bool hasTextureSampler = false;
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		
	};

}


