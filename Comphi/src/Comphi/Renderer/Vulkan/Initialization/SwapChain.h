#pragma once
#include "../Objects/ImageView.h"
#include "../Objects/MeshObject.h"
#include "SyncObjectsFactory.h"
#include "CommandPool.h"

namespace Comphi::Vulkan {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class SwapChain
	{
	public:
		SwapChain();
		void recreateSwapChain();
		void cleanUp();
		void cleanupRenderPass();
		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkSwapchainKHR swapChainObj;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		
		std::vector<ImageView> swapChainImageViews;
		ImageView swapChainDepthView;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkRenderPass renderPassObj;

		void incrementSwapChainFrame();
		int MAX_FRAMES_IN_FLIGHT = 3; //triple-buffering
		uint32_t currentFrame = 0;

		void beginRenderPassCommandBuffer(VkCommandBuffer& commandBuffer);
		void drawCommandBuffer(VkCommandBuffer& commandBuffer, MeshObject& meshObj);
		void endRenderPassCommandBuffer(VkCommandBuffer& commandBuffer);

		VkFence& getCurrentFrameFence();
		VkSemaphore& getCurrentFrameAvailableSemaphore();
		VkSemaphore& getCurrentFrameFinishedSemaphore();

		VkCommandBuffer& getCurrentFrameGraphicsCommandBuffer();
		VkCommandBuffer& getCurrentFrameTransferCommandBuffer();

	protected:
		void createFrameSyncObjects();
		SyncObjectsFactory inFlightSyncObjectsFactory;
		CommandPool inFlightCommandsPool;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		void createFrameCommandBuffers();
		std::vector<VkCommandBuffer> graphicsCommandBuffers;
		std::vector<VkCommandBuffer> transferCommandBuffers;

		void createRenderPass();
		void createFramebuffers();
		void createSwapChain();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createImageViews();
	};

}
