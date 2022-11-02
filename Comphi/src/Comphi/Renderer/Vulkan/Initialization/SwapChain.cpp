#include "cphipch.h"
#include "SwapChain.h"

namespace Comphi::Vulkan {

	SwapChain::SwapChain()
	{
		createSwapChain();
		renderPass = std::make_unique<RenderPass>(swapChainImageFormat, swapChainDepthView.imageFormat, MAX_FRAMES_IN_FLIGHT);
		createFramebuffers();
	}

	void SwapChain::createSwapChain() {

		COMPHILOG_CORE_TRACE("Creating Swapchain...");
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*GraphicsHandler::get()->physicalDevice.get(), *GraphicsHandler::get()->surface.get());

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		//Prevent waiting of img aloc from driver
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = *GraphicsHandler::get()->surface.get();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1; //1 unless stereoscopic 3D application.
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //post-processing : may use a value like VK_IMAGE_USAGE_TRANSFER_DST_BIT

		uint32_t queueFamilyIndices[] = { *GraphicsHandler::get()->graphicsQueueFamily.index, *GraphicsHandler::get()->transferQueueFamily.index }; //indices.presentFamily.value() == graphicsFamily

		//uint32_t uniqueQueueCount = 0;
		//if (indices.graphicsFamily != indices.transferFamily) uniqueQueueCount += 1;
		//if (indices.graphicsFamily != indices.presentFamily) uniqueQueueCount += 1;
		//if (indices.presentFamily != indices.transferFamily) uniqueQueueCount += 1;

		if (*GraphicsHandler::get()->graphicsQueueFamily.index != *GraphicsHandler::get()->transferQueueFamily.index) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE; // ignore color of obscured pixels (another window)

		createInfo.oldSwapchain = VK_NULL_HANDLE;
		//swap chain becomes invalid if window was resized

		if (vkCreateSwapchainKHR(*GraphicsHandler::get()->logicalDevice.get(), &createInfo, nullptr, &swapChainObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create swap chain!");
			throw std::runtime_error("failed to create swap chain");
			return;
		}

		COMPHILOG_CORE_INFO("SwapChain created Successfully!");

		vkGetSwapchainImagesKHR(*GraphicsHandler::get()->logicalDevice.get(), swapChainObj, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(*GraphicsHandler::get()->logicalDevice.get(), swapChainObj, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		createImageViews();
	}

	void SwapChain::createImageViews() {
		COMPHILOG_CORE_TRACE("Creating ImageViews...");
		swapChainImageViews.resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			swapChainImageViews[i].initSwapchainImageView(swapChainImages[i], swapChainImageFormat);
			swapChainImageViews[i].imageExtent = swapChainExtent;
		}
		swapChainDepthView = ImageView();
		swapChainDepthView.initDepthImageView(swapChainImageViews[0]);
	}

	void SwapChain::recreateSwapChain() {

		//It is possible to create a new swap chain while drawing commands on an image from the old swap chain are still in-flight. 
		//You need to pass the previous swap chain to the oldSwapChain field in the VkSwapchainCreateInfoKHR struct and destroy the old swap chain as soon as you've finished using it.
		int width = 0, height = 0;
		glfwGetFramebufferSize(GraphicsHandler::get()->windowHandle, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(GraphicsHandler::get()->windowHandle, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(*GraphicsHandler::get()->logicalDevice);

		cleanUp();
		createSwapChain();
		createFramebuffers();
	}

	void SwapChain::cleanUp() {
		short fbid = 0;
		for (auto framebuffer : swapChainFramebuffers) {
			COMPHILOG_CORE_INFO("vkDestroy Destroy framebuffer {0}", fbid++);
			vkDestroyFramebuffer(*GraphicsHandler::get()->logicalDevice, framebuffer, nullptr);
		}

		int n_img = 0;
		for (auto imageView : swapChainImageViews) {
			imageView.cleanUp();
		}

		COMPHILOG_CORE_INFO("vkDestroy Destroy Swapchain:");
		vkDestroySwapchainKHR(*GraphicsHandler::get()->logicalDevice.get(), swapChainObj, nullptr);
	}


	SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		//function-like macros require the token after the macro name to be a left parenthesis in order to expand. 
		//Wrapping the name in parentheses is really just a hack to make the next token a right parenthesis
		//without changing the meaning once you put macros aside.
		if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(GraphicsHandler::get()->windowHandle, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

#pragma region Framebuffer
	void SwapChain::createFramebuffers() {
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			std::array<VkImageView, 2> attachments = {
				swapChainImageViews[i].imageViewObj,
				swapChainDepthView.imageViewObj
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = &(*renderPass->renderPassObj);
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(*GraphicsHandler::get()->logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				COMPHILOG_CORE_FATAL("failed to create framebuffer!");
				throw std::runtime_error("failed to create framebuffer!");
				return;
			}
		}
	}

	void SwapChain::incrementSwapChainFrame()
	{
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	SwapChain::~SwapChain()
	{
		cleanUp();
		COMPHILOG_CORE_INFO("vkDestroy Destroy RenderPass");
		vkDestroyRenderPass(*GraphicsHandler::get()->logicalDevice, renderPass->renderPassObj, nullptr);
	}

#pragma endregion
}
