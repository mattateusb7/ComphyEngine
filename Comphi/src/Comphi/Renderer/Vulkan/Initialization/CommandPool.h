#pragma once
#include "../GraphicsHandler.h"


namespace Comphi::Vulkan {
	class CommandPool
	{
	protected:
		void createCommandPools();
	public:
		CommandPool();
		void createCommandBuffers(int MAX_FRAMES_IN_FLIGHT);
		std::vector<VkCommandBuffer> commandBuffers;

		VkCommandPool graphicsCommandPool;
		VkCommandPool transferCommandPool;
		~CommandPool();
	};
}

