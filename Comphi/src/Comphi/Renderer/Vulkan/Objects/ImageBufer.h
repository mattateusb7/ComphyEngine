#pragma once
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class ImageBuffer : public MemBuffer
	{
	public:
		ImageBuffer(std::string filepath, const std::shared_ptr<GraphicsHandler>& graphicsHandler,
			VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
		
		VkImage bufferObj; //override bufferType
		//<< bufferMemory;
		//<< bufferSize;
		VkExtent2D imageExtent;
		//<< graphicsHandler;
		VkFormat imageFormat;
		VkImageLayout imageLayout;

		static void copyBufferToImgBuffer(MemBuffer& srcBuffer, ImageBuffer& dstImagebuffer);
		void copyBufferToImgBuffer(MemBuffer& srcBuffer);
		bool hasStencilComponent();
		virtual void cleanUp() override;

	protected :
		void initTextureImageBuffer(std::string filepath, const std::shared_ptr<GraphicsHandler>& graphicsHandler, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
		void initImageBuffer(VkFormat format, VkImageTiling tiling, const VkImageUsageFlags& usage);
		void initDepthImageBuffer(ImageBuffer& swapChainImageBuffer, VkFormat format);
		
		ImageBuffer() = default;
		void transitionImageLayout(VkImageLayout newLayout);
	};



}