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
	}

	void GraphicsContext::SetScenes(SceneGraph& scenes)
	{
		this->scenes = &scenes;
	}

#pragma region //DEBUG!

	void GraphicsContext::updateSceneLoop() {
		
		FrameTime.Stop();

		VkCommandBuffer& commandBuffer = graphicsInstance->swapchain->getCurrentFrameGraphicsCommandBuffer();
		graphicsInstance->swapchain->beginRenderPassCommandBuffer(commandBuffer);

		//Traverse Render Scene Graph
			//foreach GlobalConstant
			//foreach Scene
			//foreach Material
			//foreach MaterialInstance
			//foreach Object
			//foreach ObjectInstance
		
		graphicsInstance->swapchain->endRenderPassCommandBuffer(commandBuffer);

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

		vkWaitForFences(graphicsInstance->logicalDevice, 1, &graphicsInstance->swapchain->getCurrentFrameFence(), VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(
			graphicsInstance->logicalDevice, 
			graphicsInstance->swapchain->swapChainObj, 
			UINT64_MAX, 
			graphicsInstance->swapchain->getCurrentFrameAvailableSemaphore(),
			VK_NULL_HANDLE, 
			&imageIndex //refers to vkGetSwapchainImagesKHR of swapchain ImageViews
		);

		if (result != VK_SUCCESS) {
			COMPHILOG_CORE_ERROR("failed to acquireNextImage!");
			//throw std::runtime_error("failed to acquireNextImage!");
			if (result == VK_SUBOPTIMAL_KHR) {
				COMPHILOG_CORE_ERROR("failed to acquire swap chain image!");
				throw std::runtime_error("failed to acquire swap chain image!");
			}
		}
		else {
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				graphicsInstance->swapchain->recreateSwapChain();
				return;
			}
		}

		// Only reset the fence if we are submitting work
		vkResetFences(graphicsInstance->logicalDevice, 1, &graphicsInstance->swapchain->getCurrentFrameFence());

		//vkResetCommandPool(graphicsInstance->logicalDevice, commandPool->graphicsCommandPool,0); 
		//if you are making multiple command buffers from one pool, resetting the pool will be quicker.
		//It can be implicitly reset when calling vkBeginCommandBuffer on the render loop
		vkResetCommandBuffer(graphicsInstance->swapchain->getCurrentFrameGraphicsCommandBuffer(), 0);

		//Scene Update
		updateSceneLoop();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { graphicsInstance->swapchain->getCurrentFrameAvailableSemaphore()};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &graphicsInstance->swapchain->getCurrentFrameGraphicsCommandBuffer();

		VkSemaphore signalSemaphores[] = { graphicsInstance->swapchain->getCurrentFrameFinishedSemaphore()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsInstance->graphicsQueue, 1, &submitInfo, graphicsInstance->swapchain->getCurrentFrameFence()) != VK_SUCCESS) {
			COMPHILOG_CORE_WARN("failed to submit draw command buffer!");
			//throw std::runtime_error("failed to submit draw command buffer!");
			//return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		
		VkSwapchainKHR swapChains[] = { graphicsInstance->swapchain->swapChainObj };
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

		//https://stackoverflow.com/questions/52915897/vulkan-at-vkqueuepresentkhr-the-validation-layer-throws-an-error-message-rega

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
			_framebufferResized = false;
			graphicsInstance->swapchain->recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to present swap chain image!");
			throw std::runtime_error("failed to present swap chain image!"); 
		}

		graphicsInstance->swapchain->incrementSwapChainFrame();
	}

	void GraphicsContext::CleanUp()
	{
		vkDeviceWaitIdle(graphicsInstance->logicalDevice);

		//TODO : create Cleanup Stack of all Instanced Engine Objects (send vk objRefs to static queue on creation?)
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