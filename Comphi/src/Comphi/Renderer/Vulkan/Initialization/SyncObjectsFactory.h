#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	static class SyncObjectsFactory
	{
	public:
		static void createSemaphores(VkSemaphore* semaphores, uint count);
		static void createFences(VkFence* fences, uint count);
	}SyncObjectsFactory;

}

