#include "cphipch.h"
#include "ImageBufer.h"

#include "../Initialization/CommandPool.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Comphi::Vulkan {

	ImageBuffer ImageBuffer::createTextureImageBuffer(IFileRef& fileref, ImageBufferSpecification& specification) {
		
		//LoadData & Setup StagingBuffer
		int texWidth, texHeight, texChannels, bufferSize;
		stbi_uc* pixels = stbi_load(fileref.getFilePath().data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		bufferSize = texWidth * texHeight * 4;//4=rgba

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		MemBuffer stagingBuffer(bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data; //copy data to staging buffer
		vkMapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(bufferSize));
		vkUnmapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory);

		stbi_image_free(pixels);

		ImageBuffer imageBuffer = ImageBuffer();
		imageBuffer.specification = specification;
		imageBuffer.imageExtent.width = static_cast<uint32_t>(texWidth);
		imageBuffer.imageExtent.height = static_cast<uint32_t>(texHeight);
		imageBuffer.allocateImageBuffer();

		imageBuffer.transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		imageBuffer.sendBufferToImgBuffer(stagingBuffer);
		imageBuffer.transitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		stagingBuffer.cleanUp();

		return imageBuffer;

	}

	ImageBuffer ImageBuffer::createDepthImageBuffer(VkExtent2D& swapchainExtent, ImageBufferSpecification& specification) {
		
		ImageBuffer imageBuffer = ImageBuffer();
		imageBuffer.specification = specification;

		imageBuffer.imageExtent = swapchainExtent;
		imageBuffer.allocateImageBuffer();
		imageBuffer.transitionImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

		return imageBuffer;
	}

	void ImageBuffer::allocateImageBuffer()
	{
		imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = imageExtent.width;
		imageInfo.extent.height = imageExtent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = specification.format;
		imageInfo.tiling = specification.tiling;
		imageInfo.initialLayout = imageLayout;
		imageInfo.usage = specification.usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//VK_SHARING_MODE_CONCURRENT;
														  //uint32_t QueueFamilyIndices[] = { graphicsHandler->transferQueueFamily.index, graphicsHandler->graphicsQueueFamily.index };
														  //imageInfo.queueFamilyIndexCount = 2;
														  //imageInfo.pQueueFamilyIndices = QueueFamilyIndices;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		if (vkCreateImage(GraphicsHandler::get()->logicalDevice, &imageInfo, nullptr, &imageBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(GraphicsHandler::get()->logicalDevice, imageBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = GraphicsHandler::findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vkCheckError(vkAllocateMemory(GraphicsHandler::get()->logicalDevice, &allocInfo, nullptr, &memoryBuffer)) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(GraphicsHandler::get()->logicalDevice, imageBuffer, memoryBuffer, 0); //Bind MemoryBuffer to ImageBuffer

	}

	void ImageBuffer::sendBufferToImgBuffer(MemBuffer& srcBuffer)
	{
		CommandBuffer commandBuffer = CommandPool::beginCommandBuffer(GraphicsCommand);

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
			imageExtent.width,
			imageExtent.height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer.buffer,
			srcBuffer.bufferObj,
			imageBuffer,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		CommandPool::endCommandBuffer(commandBuffer);
	}

	bool ImageBuffer::hasStencilComponent() {
		return specification.format == VK_FORMAT_D32_SFLOAT_S8_UINT || specification.format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void ImageBuffer::transitionImageLayout(VkImageLayout newLayout)
	{
		//TODO: add Transfer Buffer release Barrier to sync Ownership between transfer & Graphics Queues.
		//VkBufferMemoryBarrier bufferBarrier{};
		//bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		//bufferBarrier.buffer = ;

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = imageLayout;
		barrier.newLayout = newLayout;
		
		barrier.image = imageBuffer;
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

			barrier.srcQueueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;

		}
		else if (imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			queueOperation = GraphicsCommand;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; //VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			barrier.srcAccessMask = 0; //VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			barrier.srcQueueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index; //TODO: set src = TransferQueue when buffer barriers work ^^^
			barrier.dstQueueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;

		}
		else if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			queueOperation = GraphicsCommand;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			barrier.srcQueueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;
			barrier.dstQueueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		CommandBuffer commandBuffer = CommandPool::beginCommandBuffer(queueOperation);

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

		CommandPool::endCommandBuffer(commandBuffer);

		imageLayout = newLayout;

	}

	void ImageBuffer::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy ImageBuffer");
		vkDestroyImage(GraphicsHandler::get()->logicalDevice, imageBuffer, nullptr);
		vkFreeMemory(GraphicsHandler::get()->logicalDevice, memoryBuffer, nullptr);
	}
}