#pragma once
#include "../Objects/ImageView.h"
#include "RenderPass.h"

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
		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkSwapchainKHR swapChainObj;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImage> swapChainImages;
		std::vector<ImageView> swapChainImageViews;
		ImageView swapChainDepthView;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::unique_ptr<RenderPass> renderPass;

		void incrementSwapChainFrame();
		const int MAX_FRAMES_IN_FLIGHT = 3; //triple-buffering
		uint32_t currentFrame = 0;

		~SwapChain();
	protected:
		void createFramebuffers();
		void createSwapChain();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createImageViews();
	};

}
