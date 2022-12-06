#include "cphipch.h"
#include "ImageBufer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Comphi::Vulkan {
	

	void ImageBuffer::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy ImageBuffer");
		vkDestroyImage(*GraphicsHandler::get()->logicalDevice.get(), bufferObj, nullptr);
		vkFreeMemory(*GraphicsHandler::get()->logicalDevice.get(), bufferMemory, nullptr);
	}

	ImageBuffer::ImageBuffer(IFileRef& fileref, ImgSpecification spec)
	{
		std::string filepath = fileref.getFilePath();
		initTextureImageBuffer(filepath, spec);
	}

	void ImageBuffer::initDepthImageBuffer(ImageBuffer& swapChainImageBuffer, VkFormat format) {
		imageExtent = swapChainImageBuffer.imageExtent;
		ImgSpecification specs{};
		specs.format = format;
		specs.tiling = VK_IMAGE_TILING_OPTIMAL;
		specs.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		initImageBuffer(specs);
		transitionImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	void ImageBuffer::initTextureImageBuffer(std::string& filepath, ImgSpecification spec) {
		
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(filepath.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		bufferSize = texWidth * texHeight * 4;//4=rgba

		if (!pixels) {
			return throw std::runtime_error("failed to load texture image!");
		}

		MemBuffer stagingBuffer(bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data; //copy data to staging buffer
		vkMapMemory(*GraphicsHandler::get()->logicalDevice.get(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(bufferSize));
		vkUnmapMemory(*GraphicsHandler::get()->logicalDevice.get(), stagingBuffer.bufferMemory);

		stbi_image_free(pixels);

		imageExtent.width = static_cast<uint32_t>(texWidth);
		imageExtent.height = static_cast<uint32_t>(texHeight);
		initImageBuffer(spec);

		transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImgBuffer(stagingBuffer);
		transitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		//cleanup
		vkDestroyBuffer(*GraphicsHandler::get()->logicalDevice.get(), stagingBuffer.bufferObj, nullptr);
		vkFreeMemory(*GraphicsHandler::get()->logicalDevice.get(), stagingBuffer.bufferMemory, nullptr);

	}

	void ImageBuffer::initImageBuffer(ImgSpecification spec)
	{
		imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageFormat = spec.format;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = imageExtent.width;
		imageInfo.extent.height = imageExtent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = imageFormat;
		imageInfo.tiling = spec.tiling;
		imageInfo.initialLayout = imageLayout;
		imageInfo.usage = spec.usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//VK_SHARING_MODE_CONCURRENT;
														  //uint32_t QueueFamilyIndices[] = { graphicsHandler->transferQueueFamily.index, graphicsHandler->graphicsQueueFamily.index };
														  //imageInfo.queueFamilyIndexCount = 2;
														  //imageInfo.pQueueFamilyIndices = QueueFamilyIndices;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		if (vkCreateImage(*GraphicsHandler::get()->logicalDevice.get(), &imageInfo, nullptr, &bufferObj) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(*GraphicsHandler::get()->logicalDevice.get(), bufferObj, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(*GraphicsHandler::get()->physicalDevice.get(), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(*GraphicsHandler::get()->logicalDevice.get(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(*GraphicsHandler::get()->logicalDevice.get(), bufferObj, bufferMemory, 0);

	}

	void ImageBuffer::copyBufferToImgBuffer(MemBuffer& srcBuffer, ImageBuffer& dstImagebuffer)
	{
		CommandBuffer commandBuffer = GraphicsHandler::beginCommandBuffer(TransferCommand);

		VkBufferImageCopy region{}; // how the pixels are laid out in memory. For example, you could have some padding bytes between rows of the image
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		//which part of the image we want to copy the pixels.
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			dstImagebuffer.imageExtent.width,
			dstImagebuffer.imageExtent.height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer.buffer,
			srcBuffer.bufferObj,
			dstImagebuffer.bufferObj,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		GraphicsHandler::endCommandBuffer(commandBuffer);
	}

	void ImageBuffer::copyBufferToImgBuffer(MemBuffer& srcBuffer)
	{
		copyBufferToImgBuffer(srcBuffer,*this);
	}

	bool ImageBuffer::hasStencilComponent() {
		return imageFormat == VK_FORMAT_D32_SFLOAT_S8_UINT || imageFormat == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void ImageBuffer::transitionImageLayout(VkImageLayout newLayout)
	{
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = imageLayout;
		barrier.newLayout = newLayout;
		
		barrier.image = bufferObj;
		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (hasStencilComponent()) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		CommandQueueOperation queueOperation;
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			queueOperation = TransferCommand;
			
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			barrier.srcQueueFamilyIndex = *GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = *GraphicsHandler::get()->transferQueueFamily.index;

		}
		else if (imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			queueOperation = GraphicsCommand;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			barrier.srcQueueFamilyIndex = *GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = *GraphicsHandler::get()->graphicsQueueFamily.index;

		}
		else if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			queueOperation = GraphicsCommand;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			barrier.srcQueueFamilyIndex = *GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = *GraphicsHandler::get()->graphicsQueueFamily.index;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		CommandBuffer commandBuffer = GraphicsHandler::beginCommandBuffer(queueOperation);

		//https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap7.html#synchronization-access-types-supported
		//https://vulkan-tutorial.com/en/Texture_mapping/Images

		vkCmdPipelineBarrier(
			commandBuffer.buffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		GraphicsHandler::endCommandBuffer(commandBuffer);

		imageLayout = newLayout;

	}

}