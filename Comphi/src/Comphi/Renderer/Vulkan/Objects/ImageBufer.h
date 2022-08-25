#pragma once
#include "../GraphicsHandler.h"
#include "MemBuffer.h"

namespace Comphi::Vulkan {

	class ImageBuffer : public MemBuffer
	{
	public:
		
		ImageBuffer(std::string filepath, std::shared_ptr<GraphicsHandler>& graphicsHandler,VkFormat format, VkImageTiling tiling,VkImageUsageFlags usage);
		~ImageBuffer() = default;
		
		VkImage bufferObj; //override bufferType
		//<< bufferMemory;
		//<< bufferSize;
		uint32_t width;
		uint32_t height;
		//<< graphicsHandler;
		VkFormat imageFormat;
		VkImageLayout imageLayout;

		static void copyBufferToImage(MemBuffer& buffer, ImageBuffer& image, std::shared_ptr<GraphicsHandler>& graphicsHandler);
		
	protected :
		void transitionImageLayout(VkImageLayout newLayout);
	};



}