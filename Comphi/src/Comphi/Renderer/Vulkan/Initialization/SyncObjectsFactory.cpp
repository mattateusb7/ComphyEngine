#include "cphipch.h"
#include "SyncObjectsFactory.h"

namespace Comphi::Vulkan {

	void SyncObjectsFactory::createSemaphores(VkSemaphore* semaphores, uint count)
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		for (size_t i = 0; i < count; i++)
		{
			vkCheckError(vkCreateSemaphore(*GraphicsHandler::get()->logicalDevice, &semaphoreInfo, nullptr, semaphores)) {
				COMPHILOG_CORE_FATAL("failed to create semaphore!");
				throw std::runtime_error("failed to create semaphore!");
				return;
			}
		}
	}

	void SyncObjectsFactory::createFences(VkFence* fences, uint count)
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < count; i++)
		{
			vkCheckError(vkCreateFence(*GraphicsHandler::get()->logicalDevice, &fenceInfo, nullptr, fences)) {
				COMPHILOG_CORE_FATAL("failed to create semaphore!");
				throw std::runtime_error("failed to create semaphore!");
				return;
			}
		}
	}


}