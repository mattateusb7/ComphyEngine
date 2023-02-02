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
			COMPHILOG_CORE_INFO("created semaphore!");
		}
	}

	void SyncObjectsFactory::createFences(VkFence* fences, uint count, bool reset)
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
			COMPHILOG_CORE_INFO("created Fence!");
		}
		if (reset) {
			vkResetFences(GraphicsHandler::get()->logicalDevice, count, fences);
		}
	}

	void Comphi::Vulkan::SyncObjectsFactory::createSemaphore(VkSemaphore& semaphore)
	{
		createSemaphores(&semaphore, 1);
	}

	void Comphi::Vulkan::SyncObjectsFactory::createFence(VkFence& fence, bool reset)
	{
		createFences(&fence, 1, reset);
	}

	void SyncObjectsFactory::cleanup()
	{
		if(semaphores.size() > 0)
		for (int i = 0; i < semaphores.size(); i++) {
			vkDestroySemaphore(GraphicsHandler::get()->logicalDevice, *semaphores[i], nullptr);
			COMPHILOG_CORE_INFO("destroyed Semaphore!");
		}
		semaphores.clear();

		if (fences.size() > 0)
		for (int i = 0; i < fences.size(); i++) {
			vkCheckError(vkGetFenceStatus(GraphicsHandler::get()->logicalDevice,*fences[i])) {
				std::runtime_error("Invalid Fence! was the owner object destroyed ?");
			}
			vkWaitForFences(GraphicsHandler::get()->logicalDevice, 1, fences[i], VK_TRUE, UINT16_MAX);
			vkDestroyFence(GraphicsHandler::get()->logicalDevice, *fences[i], nullptr);
			COMPHILOG_CORE_INFO("destroyed Fence!");
		}
		fences.clear();

	}
}