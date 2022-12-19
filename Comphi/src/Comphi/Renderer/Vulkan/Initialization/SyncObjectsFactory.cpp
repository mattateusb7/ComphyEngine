#include "cphipch.h"
#include "SyncObjectsFactory.h"

namespace Comphi::Vulkan {

	void SyncObjectsFactory::createSemaphores(VkSemaphore* semaphores, uint count)
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		for (size_t i = 0; i < count; i++)
		{
			vkCheckError(vkCreateSemaphore(GraphicsHandler::get()->logicalDevice, &semaphoreInfo, nullptr, &semaphores[i])) {
				COMPHILOG_CORE_FATAL("failed to create semaphore!");
				throw std::runtime_error("failed to create semaphore!");
				return;
			}
			this->semaphores.push_back(&semaphores[i]);
		}
	}

	void SyncObjectsFactory::createFences(VkFence* fences, uint count)
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < count; i++)
		{
			vkCheckError(vkCreateFence(GraphicsHandler::get()->logicalDevice, &fenceInfo, nullptr, &fences[i])) {
				COMPHILOG_CORE_FATAL("failed to create semaphore!");
				throw std::runtime_error("failed to create semaphore!");
				return;
			}
			this->fences.push_back(&fences[i]);
		}
	}

	void SyncObjectsFactory::cleanup()
	{
		for (size_t i = semaphores.size(); i > 0; --i) {
			vkDestroySemaphore(GraphicsHandler::get()->logicalDevice, *semaphores[i], nullptr);
		}

		for (size_t i = fences.size(); i > 0 ; --i) {
			vkDestroyFence(GraphicsHandler::get()->logicalDevice, *fences[i], nullptr);
		}


	}
}