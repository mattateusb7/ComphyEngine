#include "cphipch.h"
#include "DescriptorPool.h"

namespace Comphi::Vulkan {

#pragma region DescriptorPool

	DescriptorPool::DescriptorPool()
	{
		/***
		/This constructor defines DescriptorPool Compatibility with Descriptor Sets Layouts
		/TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts
		***/

		int MAX_FRAMES_IN_FLIGHT = *GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT;
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		if (vkCreateDescriptorPool(*GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &descriptorPoolObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create descriptor pool!");
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	DescriptorPool::~DescriptorPool()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorPool");
		vkDestroyDescriptorPool(*Vulkan::GraphicsHandler::get()->logicalDevice, descriptorPoolObj, nullptr);
	}

#pragma endregion

}