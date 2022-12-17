#pragma once
#include "Comphi/Platform/IFileRef.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	struct ImageBufferSpecification {
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
		VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	};

	class ImageBuffer
	{
	public:
		static ImageBuffer createTextureImageBuffer(IFileRef& fileref, ImageBufferSpecification& specification); //TODO: Add rawData Initialization construct - send pixel Array as input
		static ImageBuffer createDepthImageBuffer(VkExtent2D& swapchainExtent, ImageBufferSpecification& specification);

		//Memory
		VkDeviceMemory memoryBuffer;
		VkImage imageBuffer;
		//Format
		VkExtent2D imageExtent;
		VkImageLayout imageLayout;
		ImageBufferSpecification specification;

		void cleanUp();

		ImageBuffer() = default;
	protected :

		void sendBufferToImgBuffer(MemBuffer& srcBuffer);
		//TODO: retrieveImageBufferToBuffer?

		void allocateImageBuffer();
		
		bool hasStencilComponent();
		void transitionImageLayout(VkImageLayout newLayout);
	};



}