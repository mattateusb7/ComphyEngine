#include "cphipch.h"
#include "CommandPool.h"

namespace Comphi::Vulkan {

#pragma region CommandPool

	CommandPool::CommandPool()
	{
		createCommandPools();

		GraphicsHandler::get()->setCommandPools(transferCommandPool, graphicsCommandPool);
	}

	void CommandPool::createCommandPools() {

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = *GraphicsHandler::get()->graphicsQueueFamily.index;

		if (vkCreateCommandPool(*GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &graphicsCommandPool) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create command pool!");
			throw std::runtime_error("failed to create command pool!");
			return;
		}

		VkCommandPoolCreateInfo poolInfoTransfer{};
		poolInfoTransfer.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfoTransfer.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
		poolInfoTransfer.queueFamilyIndex = *GraphicsHandler::get()->transferQueueFamily.index;

		if (vkCreateCommandPool(*GraphicsHandler::get()->logicalDevice, &poolInfoTransfer, nullptr, &transferCommandPool) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create transfer command pool!");
			throw std::runtime_error("failed to create transfer command pool!");
			return;
		}

	}

	void CommandPool::createCommandBuffers(int MAX_FRAMES_IN_FLIGHT) {

		commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = graphicsCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());


		if (vkAllocateCommandBuffers(*GraphicsHandler::get()->logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to allocate command buffers!");
			throw std::runtime_error("failed to allocate command buffers!");
			return;
		}
	}

	CommandPool::~CommandPool()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy transferCommandPool");
		vkDestroyCommandPool(*GraphicsHandler::get()->logicalDevice, transferCommandPool, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy graphicsCommandPool");
		vkDestroyCommandPool(*GraphicsHandler::get()->logicalDevice, graphicsCommandPool, nullptr);
	}

#pragma endregion

}
