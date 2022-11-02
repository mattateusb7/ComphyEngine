#pragma once
#include "../GraphicsHandler.h"

namespace Comphi::Vulkan {

	class SyncObjects
	{
	public:
		SyncObjects(int MAX_FRAMES_IN_FLIGHT);
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		~SyncObjects();
	protected :
		void createSyncObjects(int MAX_FRAMES_IN_FLIGHT);
		int last_MAX_FRAMES_IN_FLIGHT;
	};

}

