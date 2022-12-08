#include "cphipch.h"
#include "CommandPool.h"

namespace Comphi::Vulkan {

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

		vkCheckError(vkCreateCommandPool(*GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &graphicsCommandPool)) {
			COMPHILOG_CORE_FATAL("failed to create command pool!");
			throw std::runtime_error("failed to create command pool!");
			return;
		}

		VkCommandPoolCreateInfo poolInfoTransfer{};
		poolInfoTransfer.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfoTransfer.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
		poolInfoTransfer.queueFamilyIndex = *GraphicsHandler::get()->transferQueueFamily.index;

		vkCheckError(vkCreateCommandPool(*GraphicsHandler::get()->logicalDevice, &poolInfoTransfer, nullptr, &transferCommandPool)) {
			COMPHILOG_CORE_FATAL("failed to create transfer command pool!");
			throw std::runtime_error("failed to create transfer command pool!");
			return;
		}

	}

	void CommandPool::createCommandBuffers(int MAX_FRAMES_IN_FLIGHT) {

		graphicsCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		transferCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo_graphics{};
		allocInfo_graphics.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo_graphics.commandPool = graphicsCommandPool;
		allocInfo_graphics.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo_graphics.commandBufferCount = static_cast<uint32_t>(graphicsCommandBuffers.size());


		vkCheckError(vkAllocateCommandBuffers(*GraphicsHandler::get()->logicalDevice, &allocInfo_graphics, graphicsCommandBuffers.data())) {
			COMPHILOG_CORE_FATAL("failed to allocate command buffers!");
			throw std::runtime_error("failed to allocate command buffers!");
			return;
		}
		COMPHILOG_CORE_INFO("Allocated {0} GraphicsCommandBuffers from graphicsCommandPool", MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo_transfer{};
		allocInfo_transfer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo_transfer.commandPool = transferCommandPool;
		allocInfo_transfer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo_transfer.commandBufferCount = static_cast<uint32_t>(transferCommandBuffers.size());


		vkCheckError(vkAllocateCommandBuffers(*GraphicsHandler::get()->logicalDevice, &allocInfo_transfer, transferCommandBuffers.data())) {
			COMPHILOG_CORE_FATAL("failed to allocate command buffers!");
			throw std::runtime_error("failed to allocate command buffers!");
			return;
		}
		COMPHILOG_CORE_INFO("Allocated {0} TransferCommandBuffers from graphicsCommandPool", MAX_FRAMES_IN_FLIGHT);
	}

	void CommandPool::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy transferCommandPool");
		vkDestroyCommandPool(*GraphicsHandler::get()->logicalDevice, transferCommandPool, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy graphicsCommandPool");
		vkDestroyCommandPool(*GraphicsHandler::get()->logicalDevice, graphicsCommandPool, nullptr);
	}

}
