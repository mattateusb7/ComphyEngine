#include "cphipch.h"
#include "GraphicsContext.h"

#include "Initialization/SyncObjectsFactory.h"

namespace Comphi::Vulkan {

	GraphicsContext::GraphicsContext(GLFWwindow& windowHandle)
	{
		GraphicsHandler::get()->windowHandle = &windowHandle;
		COMPHILOG_CORE_ASSERT(&windowHandle, "Window Handle is NULL!");
	}

/*INFO
//! VULKAN Guide: https://vulkan-tutorial.com/
//! VULKAN Guide2: https://vkguide.dev/
//! VULKAN Map	https://github.com/David-DiGioia/vulkan-diagrams
//! VULKAN SPIR Compile : https://www.khronos.org/spir/
*/

	void GraphicsContext::Init()
	{
		graphicsInstance = std::make_unique<GraphicsInstance>();
		commandPool = std::make_unique<CommandPool>();
		swapchain = std::make_unique<SwapChain>();
		commandPool->createCommandBuffers(swapchain->MAX_FRAMES_IN_FLIGHT);
		createSyncObjects();
	}

	void GraphicsContext::SetScenes(MultiScene& scenes)
	{
		this->scenes = &scenes;
	}

	void GraphicsContext::createSyncObjects() {

		imageAvailableSemaphores.resize(swapchain->MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(swapchain->MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(swapchain->MAX_FRAMES_IN_FLIGHT);

		SyncObjectsFactory.createSemaphores(&imageAvailableSemaphores[0], swapchain->MAX_FRAMES_IN_FLIGHT);
		SyncObjectsFactory.createSemaphores(&renderFinishedSemaphores[0], swapchain->MAX_FRAMES_IN_FLIGHT);
		SyncObjectsFactory.createFences(&inFlightFences[0], swapchain->MAX_FRAMES_IN_FLIGHT);
		
		COMPHILOG_CORE_INFO("semaphores created Successfully!");
	}

#pragma region //DEBUG!

	void GraphicsContext::updateSceneLoop() {

		FrameTime.Stop();

		for (size_t i = 0; i < scenes->size(); i++)
		{
			Scene* ThisScene = (*scenes)[i].get();
			if (ThisScene == nullptr) continue;
			//Action UpdateCallback
			for (size_t i = 0; i < ThisScene->sceneObjects.size(); i++)
			{
				ThisScene->sceneObjects[i]->action.updateCallback(FrameTime, 0);
			}

			for (size_t i = 0; i < ThisScene->sceneObjects.size(); i++)
			{
				//Update Uniform Buffers MVP_UBOs per GameObject :
				UniformBufferObject ubo{};
				ubo.model = ThisScene->sceneObjects[i]->transform.getModelMatrix();
				ubo.view = ThisScene->sceneCamera->getViewMatrix();
				ubo.proj = ThisScene->sceneCamera->getProjectionMatrix();

				((MeshObject*)(ThisScene->sceneObjects[i]->mesh.get()))->updateMVP(swapchain->currentFrame); //TODO: This is not OK
				ThisScene->sceneObjects[i]->action.startCallback(FrameTime, 0);

				//Draw Command Buffer Submission:
				//One command Buffer / render Pass , per Object 
				//TODO: InstancedObjects
				swapchain->recordCommandBuffer(commandPool->graphicsCommandBuffers[swapchain->currentFrame], *((MeshObject*)(ThisScene->sceneObjects[i]->mesh.get())), swapchain->currentFrame);  //TODO: This is not OK either

			}
		}
		
		FrameTime.Start();

	}

#pragma endregion

	void GraphicsContext::Draw()
	{
		//Wait for the previous frame to finish
		//Acquire an image from the swap chain
		//Record a command buffer which draws the scene onto that image
		//Submit the recorded command buffer
		//Present the swap chain image

		vkWaitForFences(graphicsInstance->logicalDevice, 1, &inFlightFences[swapchain->currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(graphicsInstance->logicalDevice, swapchain->swapChainObj, UINT64_MAX, imageAvailableSemaphores[swapchain->currentFrame], VK_NULL_HANDLE, &imageIndex);
		if (result != VK_SUCCESS) {
			COMPHILOG_CORE_ERROR("failed to acquireNextImage!");
			//throw std::runtime_error("failed to acquireNextImage!");
		}
		else {
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				swapchain->recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				COMPHILOG_CORE_ERROR("failed to acquire swap chain image!");
				throw std::runtime_error("failed to acquire swap chain image!");
			}
		}

		// Only reset the fence if we are submitting work
		vkResetFences(graphicsInstance->logicalDevice, 1, &inFlightFences[swapchain->currentFrame]);

		//vkResetCommandPool(graphicsInstance->logicalDevice, commandPool->graphicsCommandPool,0); 
		//if you are making multiple command buffers from one pool, resetting the pool will be quicker.
		vkResetCommandBuffer(commandPool->graphicsCommandBuffers[swapchain->currentFrame], 0);

		//Scene Update
		updateSceneLoop();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[swapchain->currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandPool->graphicsCommandBuffers[swapchain->currentFrame];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[swapchain->currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsInstance->graphicsQueue, 1, &submitInfo, inFlightFences[swapchain->currentFrame]) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to submit draw command buffer!");
			throw std::runtime_error("failed to submit draw command buffer!");
			return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		
		VkSwapchainKHR swapChains[] = { swapchain->swapChainObj };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional error handling

		result = vkQueuePresentKHR(graphicsInstance->presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			swapchain->recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to present swap chain image!");
			throw std::runtime_error("failed to present swap chain image!");
			return;
		}

		swapchain->incrementSwapChainFrame();
	}

	void GraphicsContext::CleanUp()
	{
		vkDeviceWaitIdle(graphicsInstance->logicalDevice);

		swapchain->~SwapChain();
		commandPool->~CommandPool();
		
		COMPHILOG_CORE_INFO("vkDestroy Destroy Semaphores & Frames in flight");
		for (size_t i = 0; i < swapchain->MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(*GraphicsHandler::get()->logicalDevice, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(*GraphicsHandler::get()->logicalDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(*GraphicsHandler::get()->logicalDevice, inFlightFences[i], nullptr);
		}

		graphicsInstance->~GraphicsInstance();
		GraphicsHandler::get()->DeleteStatic();
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		ResizeFramebuffer(x, y);
	}

	void GraphicsContext::ResizeFramebuffer(uint x, uint y)
	{
		framebufferResized = true;
	}
}