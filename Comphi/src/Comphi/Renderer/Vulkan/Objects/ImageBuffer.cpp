#include "cphipch.h"
#include "ImageBufer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Comphi::Vulkan {

	void ImageBuffer::initTextureImageBuffer(IFileRef& fileref, ImageBufferSpecification& specification) {
		
		//LoadData & Setup StagingBuffer
		int texWidth, texHeight, texChannels, bufferSize;
		stbi_uc* pixels = stbi_load(fileref.getFilePath().data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		bufferSize = texWidth * texHeight * 4;//4=rgba

		if (!pixels) {
			std::runtime_error("failed to load texture image!");
			COMPHILOG_CORE_ERROR("failed to load texture image!");
			return;
		}

		MemBuffer stagingBuffer(bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data; //copy data to staging buffer
		vkMapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(bufferSize));
		vkUnmapMemory(GraphicsHandler::get()->logicalDevice, stagingBuffer.bufferMemory);

		stbi_image_free(pixels);

		//Allocate and bind imageBuffer & BufferMemory
		specification = specification;
		imageExtent.width = static_cast<uint32_t>(texWidth);
		imageExtent.height = static_cast<uint32_t>(texHeight);
		allocateImageBuffer();

		//TODO : Abstract ownership changes (below)
		//LayoutChange SyncObjects
		layoutChangeSyncObjects.createFence(layoutChangeFence);
		layoutChangeSyncObjects.createSemaphore(ownershipChangeSemaphore);

		//command buffer that releases the image: Transition ImageLayout and ownership to Transfer Queue
		CommandBuffer transferCommand = CommandPool::beginCommandBuffer(TransferCommand);
		transferCommand.fence = &layoutChangeFence;
		transferCommand.signalSemaphore = &ownershipChangeSemaphore;
		transitionImageLayout(transferCommand, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		sendBufferToImgBuffer(stagingBuffer, transferCommand);
		transitionImageLayout(transferCommand, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_NONE);
		CommandPool::endCommandBuffer(transferCommand);
		
		stagingBuffer.cleanUp();

		//command buffer that acquires the image : Transition ImageLayout and ownership from Transfer Queue to Graphics Queue
		CommandBuffer graphicsCommand = CommandPool::beginCommandBuffer(GraphicsCommand);
		graphicsCommand.fence = &layoutChangeFence;
		graphicsCommand.waitSemaphore = &ownershipChangeSemaphore;
		VkPipelineStageFlags waitDstStageMask[1] = { VK_PIPELINE_STAGE_TRANSFER_BIT }; //wait for transfer copy ^
		graphicsCommand.waitDstStageMask = waitDstStageMask;
		transitionImageLayout(graphicsCommand, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_SHADER_READ_BIT);
		CommandPool::endCommandBuffer(graphicsCommand);
	}

	void ImageBuffer::initDepthImageBuffer(VkExtent2D& swapchainExtent, ImageBufferSpecification& specification) {
		
		specification = specification;
		imageExtent = swapchainExtent;
		allocateImageBuffer();
		
		//LayoutChange SyncObjects
		//layoutChangeSyncObjects.createFence(layoutChangeFence);

		CommandBuffer graphicsCommand = CommandPool::beginCommandBuffer(GraphicsCommand);
		//graphicsCommand.fence = &layoutChangeFence;
		transitionImageLayout(graphicsCommand, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		CommandPool::endCommandBuffer(graphicsCommand);
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
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;											  
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		//imageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		//uint32_t QueueFamilyIndices[] = { GraphicsHandler::get()->transferQueueFamily.index, GraphicsHandler::get()->graphicsQueueFamily.index };
		//imageInfo.queueFamilyIndexCount = 2;
		//imageInfo.pQueueFamilyIndices = QueueFamilyIndices;
	

		if (vkCreateImage(GraphicsHandler::get()->logicalDevice, &imageInfo, nullptr, &imageReference) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(GraphicsHandler::get()->logicalDevice, imageReference, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = MemBuffer::findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vkCheckError(vkAllocateMemory(GraphicsHandler::get()->logicalDevice, &allocInfo, nullptr, &memoryBuffer)) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(GraphicsHandler::get()->logicalDevice, imageReference, memoryBuffer, 0); //Bind MemoryBuffer to imageRef
	}

	void ImageBuffer::sendBufferToImgBuffer(MemBuffer& srcBuffer, CommandBuffer& commandBuffer)
	{
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
			imageReference,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

	}

	bool ImageBuffer::hasStencilComponent() {
		return specification.format == VK_FORMAT_D32_SFLOAT_S8_UINT || specification.format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void ImageBuffer::transitionImageLayout(CommandBuffer& commandBuffer, VkImageLayout newLayout, VkAccessFlags accessMask)
	{

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = imageLayout;
		barrier.newLayout = newLayout;
		barrier.image = imageReference;

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

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;
		if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {

			barrier.srcAccessMask = VK_ACCESS_NONE;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

			barrier.srcQueueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;
			
			imageLayout = newLayout;

		}
		else if (accessMask == VK_ACCESS_NONE && imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_NONE;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT; //TRANSFER OWNERSHIP barrier, don't save layout change yet
			destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			barrier.srcQueueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;

		}		
		else if (accessMask == VK_ACCESS_SHADER_READ_BIT && imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

			barrier.srcAccessMask = VK_ACCESS_NONE;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			barrier.srcQueueFamilyIndex = GraphicsHandler::get()->transferQueueFamily.index;
			barrier.dstQueueFamilyIndex = GraphicsHandler::get()->graphicsQueueFamily.index;
			
			imageLayout = newLayout;

		}
		else if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

			barrier.srcAccessMask = VK_ACCESS_NONE;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //GraphicsHandler::get()->graphicsQueueFamily.index;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //GraphicsHandler::get()->graphicsQueueFamily.index;
			
			imageLayout = newLayout;

		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		//https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap7.html#synchronization-access-types-supported
		//https://vulkan-tutorial.com/en/Texture_mapping/Images
		//https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples-(Legacy-synchronization-APIs)
		
		vkCmdPipelineBarrier(
			commandBuffer.buffer,
			sourceStage, 
			destinationStage,
			0,
			0, VK_NULL_HANDLE,
			0, VK_NULL_HANDLE,
			1, &barrier
		);

	}

	void ImageBuffer::cleanUp()
	{
		layoutChangeSyncObjects.cleanup();
		
		COMPHILOG_CORE_INFO("vkDestroy Destroy ImageBuffer");
		vkFreeMemory(GraphicsHandler::get()->logicalDevice, memoryBuffer, nullptr);
		vkDestroyImage(GraphicsHandler::get()->logicalDevice, imageReference, nullptr);
	}
}