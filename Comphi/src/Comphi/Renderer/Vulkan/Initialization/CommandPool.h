#pragma once
#include "../GraphicsHandler.h"


namespace Comphi::Vulkan {

	//Allocator for CommandBuffers
	class CommandPool
	{
	protected:
		void createCommandPools();
	public:
		CommandPool();
		void createCommandBuffers(int MAX_FRAMES_IN_FLIGHT);
		std::vector<VkCommandBuffer> graphicsCommandBuffers;
		std::vector<VkCommandBuffer> transferCommandBuffers;

		VkCommandPool graphicsCommandPool;
		VkCommandPool transferCommandPool;
		~CommandPool();
	};
}

