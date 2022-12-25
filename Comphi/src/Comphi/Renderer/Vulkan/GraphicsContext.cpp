#include "cphipch.h"
#include "GraphicsContext.h"

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
		syncObjectsFactory = std::make_unique<SyncObjectsFactory>();
		createSyncObjects();
		createCommandBuffers();
	}

	void GraphicsContext::SetScenes(MultiScene& scenes)
	{
		this->scenes = &scenes;
	}

	void GraphicsContext::createCommandBuffers() {
		graphicsCommandBuffers.resize(swapchain->MAX_FRAMES_IN_FLIGHT);
		transferCommandBuffers.resize(swapchain->MAX_FRAMES_IN_FLIGHT);

		commandPool->allocateGraphicsCommandBuffer(&graphicsCommandBuffers[0], graphicsCommandBuffers.size());
		commandPool->allocateTransferCommandBuffer(&transferCommandBuffers[0], transferCommandBuffers.size());
	}

	void GraphicsContext::createSyncObjects() {

		imageAvailableSemaphores.resize(swapchain->MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(swapchain->MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(swapchain->MAX_FRAMES_IN_FLIGHT);

		syncObjectsFactory->createSemaphores(&imageAvailableSemaphores[0], swapchain->MAX_FRAMES_IN_FLIGHT);
		syncObjectsFactory->createSemaphores(&renderFinishedSemaphores[0], swapchain->MAX_FRAMES_IN_FLIGHT);
		syncObjectsFactory->createFences(&inFlightFences[0], swapchain->MAX_FRAMES_IN_FLIGHT, false);
	}

#pragma region //DEBUG!

	void GraphicsContext::updateSceneLoop() {
		
		FrameTime.Stop();

		VkCommandBuffer& commandBuffer = graphicsCommandBuffers[swapchain->currentFrame];
		swapchain->beginRenderPassCommandBuffer(commandBuffer);

		for (size_t i = 0; i < scenes->size(); i++)
		{
			Scene* ThisScene = (*scenes)[i].get();
			if (ThisScene == nullptr) continue;

			//Action UpdateCallback
			for (size_t i = 0; i < ThisScene->sceneObjects.size(); i++)
			{
				ActionHandle handle = ThisScene->sceneObjects[i]->action;
				if (handle.updateCallback) {
					handle.updateCallback(FrameTime, 0);
				}
			}


			//Update Uniform Buffers MVP_UBOs per GameObject & submit Draw Command Buffer
			for (size_t i = 0; i < ThisScene->sceneObjects.size(); i++)
			{
				UniformBufferObject ubo{};
				ubo.model = ThisScene->sceneObjects[i]->transform.getModelMatrix();
				ubo.view = ThisScene->sceneCamera->getViewMatrix();
				ubo.proj = ThisScene->sceneCamera->getProjectionMatrix();

				static_cast<MeshObject*>(ThisScene->sceneObjects[i]->mesh.get())->updateMVP(ubo, swapchain->currentFrame);
				//ThisScene->sceneObjects[i]->action.startCallback(FrameTime, 0);

				//TODO: InstancedObjects
				//Draw Command Buffer Submission:
				//One command Buffer / render Pass , per Object 
				//we are able to send multiple vkCmdDraw inside same renderpass Command ! : https://vkguide.dev/docs/chapter-3/scene_management/
				swapchain->drawCommandBuffer(commandBuffer, *static_cast<MeshObject*>(ThisScene->sceneObjects[i]->mesh.get()));  //TODO: This is not OK either
			}
		}
		
		swapchain->endRenderPassCommandBuffer(commandBuffer);

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
		vkResetCommandBuffer(graphicsCommandBuffers[swapchain->currentFrame], 0);

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
		submitInfo.pCommandBuffers = &graphicsCommandBuffers[swapchain->currentFrame];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[swapchain->currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsInstance->graphicsQueue, 1, &submitInfo, inFlightFences[swapchain->currentFrame]) != VK_SUCCESS) {
			COMPHILOG_CORE_WARN("failed to submit draw command buffer!");
			//throw std::runtime_error("failed to submit draw command buffer!");
			//return;
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

		/* TODO : Fix ImageView Presentation ready-ness on Framebuffer Resize
		* VK_validation layer: Validation Error: [ VUID-VkPresentInfoKHR-pImageIndices-01296 ] 
		Object 0: handle = 0x1dec32603e0, type = VK_OBJECT_TYPE_QUEUE; | MessageID = 0xc7aabc16 | vkQueuePresentKHR(): 
		pSwapchains[0] images passed to present must be in layout VK_IMAGE_LAYOUT_PRESENT_SRC_KHR or VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR but is in VK_IMAGE_LAYOUT_UNDEFINED. 
		The Vulkan spec states: Each element of pImageIndices must be the index of a presentable image acquired from the swapchain 
		specified by the corresponding element of the pSwapchains array, 
		and the presented image subresource must be in the VK_IMAGE_LAYOUT_PRESENT_SRC_KHR layout 
		at the time the operation is executed on a VkDevice (https://github.com/KhronosGroup/Vulkan-Docs/search?q=)VUID-VkPresentInfoKHR-pImageIndices-01296)
		*/

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
			_framebufferResized = false;
			swapchain->recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to present swap chain image!");
			throw std::runtime_error("failed to present swap chain image!"); 
		}

		swapchain->incrementSwapChainFrame();
	}

	void GraphicsContext::CleanUp()
	{
		vkDeviceWaitIdle(graphicsInstance->logicalDevice);

		//TODO : create Cleanup Stack of all Instanced Engine Objects (send vk objRefs to static queue on creation)

		swapchain->cleanUp();
		swapchain->cleanupRenderPass();
		commandPool->cleanUp();
		
		//TODO: we need a way to track & clean Shaders & Graphics Pipelines (Game Instances)

		syncObjectsFactory->cleanup();
		
		GraphicsHandler::get()->DeleteStatic();
		graphicsInstance->cleanUp();
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		ResizeFramebuffer(x, y);
	}

	void GraphicsContext::ResizeFramebuffer(uint x, uint y)
	{
		_framebufferResized = true;
	}
}