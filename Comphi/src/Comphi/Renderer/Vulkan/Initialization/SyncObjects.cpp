#include "cphipch.h"
#include "SyncObjects.h"

namespace Comphi::Vulkan {

#pragma region SyncObjects
	void SyncObjects::createSyncObjects(int MAX_FRAMES_IN_FLIGHT) {
		last_MAX_FRAMES_IN_FLIGHT = MAX_FRAMES_IN_FLIGHT;
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(*GraphicsHandler::get()->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(*GraphicsHandler::get()->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(*GraphicsHandler::get()->logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

				COMPHILOG_CORE_FATAL("failed to create synchronization objects for a frame!");
				throw std::runtime_error("failed to create synchronization objects for a frame!");
				return;
			}
		}
		COMPHILOG_CORE_INFO("semaphores created Successfully!");
	}
	SyncObjects::~SyncObjects()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy Semaphores & Frames in flight");
		for (size_t i = 0; i < last_MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(*GraphicsHandler::get()->logicalDevice, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(*GraphicsHandler::get()->logicalDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(*GraphicsHandler::get()->logicalDevice, inFlightFences[i], nullptr);
		}
	}
#pragma endregion

}