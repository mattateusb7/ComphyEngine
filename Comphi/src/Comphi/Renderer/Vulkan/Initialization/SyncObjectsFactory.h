#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class SyncObjectsFactory
	{
	public:
		SyncObjectsFactory() = default;

		void createSemaphores(VkSemaphore* semaphores, uint count);
		void createFences(VkFence* fences, uint count, bool reset = true);

		std::vector<VkSemaphore*> semaphores = std::vector<VkSemaphore*>();
		std::vector<VkFence*> fences = std::vector<VkFence*>();

		void cleanup();

	};

}

