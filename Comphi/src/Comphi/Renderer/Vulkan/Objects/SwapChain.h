#pragma once
#include "ImageView.h"

namespace Comphi::Vulkan {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class SwapChain
	{
	public:
		void createSwapChain(const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		SwapChain(const std::shared_ptr<GraphicsHandler>& graphicsHandler);
		void cleanUp();

		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkSwapchainKHR swapChainObj;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImage> swapChainImages;
		std::vector<ImageView> swapChainImageViews;

		std::shared_ptr<GraphicsHandler> graphicsHandler;

	protected:
		void createImageViews();
	};

}
