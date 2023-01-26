#pragma once
#include "Comphi/Platform/IFileRef.h"
#include "MemBuffer.h"
#include "../Initialization/CommandPool.h"
#include "../Initialization/SyncObjectsFactory.h"

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
		void initTextureImageBuffer(IFileRef& fileref, ImageBufferSpecification& specification); //TODO: Add rawData Initialization construct - send pixel Array as input
		void initDepthImageBuffer(VkExtent2D& swapchainExtent, ImageBufferSpecification& specification);

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
		SyncObjectsFactory layoutChangeSyncObjects;
		VkSemaphore ownershipChangeSemaphore;
		VkFence layoutChangeFence;

		void allocateImageBuffer();
		bool hasStencilComponent();
		void transitionImageLayout(CommandBuffer& commandBuffer, VkImageLayout newLayout, VkAccessFlags accessMask = 0U);
		void sendBufferToImgBuffer(MemBuffer& srcBuffer, CommandBuffer& commandBuffer);
	};



}