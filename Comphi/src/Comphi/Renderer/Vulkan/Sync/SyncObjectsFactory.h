#pragma once
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"

namespace Comphi::Vulkan {

	class SyncObjectsFactory
	{
	public:
		SyncObjectsFactory() = default;

		void createSemaphore(VkSemaphore& semaphore);
		void createSemaphores(VkSemaphore* semaphores, uint count);
		void createFences(VkFence* fences, uint count, bool reset = true);
		void createFence(VkFence& fence, bool reset = true);

		std::vector<VkSemaphore*> semaphores = std::vector<VkSemaphore*>();
		std::vector<VkFence*> fences = std::vector<VkFence*>();

		void cleanup();

	};

}

