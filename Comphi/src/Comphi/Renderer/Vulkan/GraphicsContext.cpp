#include "cphipch.h"
#include "GraphicsContext.h"

#include "Comphi/Renderer/GraphicsAPI.h"
#include "Comphi/Platform/Windows/FileRef.h"
#include "Comphi/Events/Event.h"

#include <optional>
#include <set>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace Comphi::Vulkan {

	static VkInstance instance;

	GraphicsContext::GraphicsContext(GLFWwindow& windowHandle) : m_WindowHandle(&windowHandle)
	{
		COMPHILOG_CORE_ASSERT(m_WindowHandle, "Window Handle is NULL!");
	}

//! VULKAN Guide: https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Logical_device_and_queues
//! VULKAN Map	https://github.com/David-DiGioia/vulkan-diagrams

	void GraphicsContext::Init()
	{
		createVKInstance();
#ifndef NDEBUG
		setupDebugMessenger();
#endif //!NDEBUG
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
	}

	void GraphicsContext::createSurface() {
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(m_WindowHandle);
		createInfo.hinstance = GetModuleHandle(nullptr);

		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("Failed to create window surface!");
		}

		if (glfwCreateWindowSurface(instance, m_WindowHandle, nullptr, &surface) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("Failed to create window surface!");
		}

		COMPHILOG_CORE_INFO("vk_surface window creation successful!");
	}

	void GraphicsContext::createVKInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "[Comphi Engine]";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Comphi";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredGLFWExtensions();
		//!TODO: VALIDATION LAYERS
#ifdef NDEBUG
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#else
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		
		if (!GraphicsContext::checkValidationLayerSupport(validationLayers)) {
			COMPHILOG_CORE_FATAL("validation layers requested, but not available!");
		}

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

#endif //!NDEBUG

		if (extensions.size() > 0) {
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();
		}

		if (&createInfo == nullptr || &instance == nullptr) {
			COMPHILOG_CORE_FATAL("Null pointer passed to vkCreateInstance!");
		}
		else if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create vkinstance!");
		}
		COMPHILOG_CORE_INFO("vk instance creation successful!");
	}

#ifndef NDEBUG
	bool GraphicsContext::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {

		COMPHILOG_CORE_TRACE("Requesting Validation Layers:");

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					COMPHILOG_CORE_TRACE(layerName);
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		COMPHILOG_CORE_INFO("Validation Layers found!");
		return true;
	}

	void GraphicsContext::setupDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to set up debug messenger!");
		}
		COMPHILOG_CORE_INFO("DebugMessenger setup successful!");
	}

	void GraphicsContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsContext::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		switch (messageSeverity)
		{
//#define VERBOSE_DEBUG
#ifdef VERBOSE_DEBUG
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			COMPHILOG_CORE_TRACE("VK_validation layer: {0}", pCallbackData->pMessage);
			break;
#endif //!VERBOSE_DEBUG
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			COMPHILOG_CORE_INFO("VK_validation layer: {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			COMPHILOG_CORE_WARN("VK_validation layer: {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			COMPHILOG_CORE_ERROR("VK_validation layer: {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			COMPHILOG_CORE_FATAL("VK_validation layer: {0}", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}

	VkResult GraphicsContext::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void GraphicsContext::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

#endif //!NDEBUG

	std::vector<const char*> GraphicsContext::getRequiredGLFWExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		if (!checkGLFWRequiredInstanceExtensions(glfwExtensions, glfwExtensionCount)) {
			return std::vector<const char*>();
		}

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return extensions;
	}

	bool GraphicsContext::checkGLFWRequiredInstanceExtensions(const char**& glfwExtensions, uint32_t& glfwExtensionCount) {

		COMPHILOG_CORE_TRACE("Requesting GLFW RequiredInstanceExtensions:");

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		COMPHILOG_CORE_TRACE("GLFW_extensions supported: {0}", glfwExtensionCount);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		COMPHILOG_CORE_TRACE("Vk_extensions supported: {0}", extensionCount);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::vector<std::string>extensionsRequired;

		for (uint32_t i = 0; i < glfwExtensionCount; i++) {
			const std::string ext = glfwExtensions[i];
			extensionsRequired.push_back(ext);
		}

		for (const auto& extension : extensions) {

			const std::string vk_ext = extension.extensionName;
			auto found = std::find(extensionsRequired.begin(), extensionsRequired.end(), vk_ext);

			if (found != extensionsRequired.end()) {
				COMPHILOG_CORE_INFO("GLFW REQUIRED EXTENSION FOUND: " + *found);
				extensionsRequired.erase(found);
				continue;
			}
			/*List All Extensions*/
			//COMPHILOG_CORE_INFO(vk_ext);
		}

		if (!extensionsRequired.empty()) {
			for (std::string& ext : extensionsRequired) {
				COMPHILOG_CORE_FATAL("GLFW REQUIRED EXTENSION MISSING: " + ext);
				return false;
			}
		}
		return true;
	}

	void GraphicsContext::pickPhysicalDevice() {
		physicalDevice = VK_NULL_HANDLE;

		COMPHILOG_CORE_TRACE("Queue PhysicalDevices...");
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		
		if (deviceCount == 0) {
			COMPHILOG_CORE_FATAL("failed to find GPUs with Vulkan support!");
			return;
		}
		COMPHILOG_CORE_INFO("PhysicalDevices found!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				COMPHILOG_CORE_INFO("Suitable Device Found!");
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			COMPHILOG_CORE_FATAL("failed to find a suitable GPU!");
			return;
		}
		COMPHILOG_CORE_INFO("PhysicalDevice setup successful!");
	}

	GraphicsContext::QueueFamilyIndices GraphicsContext::findQueueFamilies(VkPhysicalDevice device) {
		
		COMPHILOG_CORE_TRACE("Requesting QueueFamilies...");
		QueueFamilyIndices indices;
		// Assign index to queue families that could be found

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		if (queueFamilyCount == 0) {
			COMPHILOG_CORE_FATAL("failed to queueFamilies for device!");
			return indices;
		}

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport) {
				indices.presentFamily = i;
			}
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			if (indices.isComplete()) {
				COMPHILOG_CORE_INFO("queueFamily found!");
				break;
			}
			i++;
		}

		return indices;
	}

	bool GraphicsContext::isDeviceSuitable(VkPhysicalDevice device) {
		COMPHILOG_CORE_TRACE("Checking Physical device suitability...");
		//VkPhysicalDeviceProperties deviceProperties;
		//VkPhysicalDeviceFeatures deviceFeatures;
		//vkGetPhysicalDeviceProperties(device, &deviceProperties);
		//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		//
		//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
		
		// - - -

		//for custom / automatic device selection https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Physical_devices_and_queue_families
																	   
		/*VkPhysicalDeviceProperties deviceProperties;				   */
		/*vkGetPhysicalDeviceProperties(device, &deviceProperties);	   */
		/*															   */
		/*VkPhysicalDeviceFeatures deviceFeatures;					   */
		/*vkGetPhysicalDeviceFeatures(device, &deviceFeatures);		   */

		GraphicsContext::QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool GraphicsContext::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}
#pragma region Logical Device
	void GraphicsContext::createLogicalDevice() {
		if (physicalDevice == VK_NULL_HANDLE) return;

		COMPHILOG_CORE_TRACE("Creating Logical Device...");

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{}; //Default all VK_FALSE

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifndef NDEBUG 
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif //!NDEBUG

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create logical device!");
		}
		COMPHILOG_CORE_INFO("Logical Device creation successful!");

		vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);

		COMPHILOG_CORE_INFO("Graphics Queue request successful!");

		vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);

		COMPHILOG_CORE_INFO("Present Queue request successful!");
	}

#pragma endregion
#pragma region SwapChain

	GraphicsContext::SwapChainSupportDetails GraphicsContext::querySwapChainSupport(VkPhysicalDevice device) {
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

	VkSurfaceFormatKHR GraphicsContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR GraphicsContext::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D GraphicsContext::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		//function-like macros require the token after the macro name to be a left parenthesis in order to expand. 
		//Wrapping the name in parentheses is really just a hack to make the next token a right parenthesis
		//without changing the meaning once you put macros aside.
		if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) { 
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(m_WindowHandle, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void GraphicsContext::createSwapChain() {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

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
		createInfo.surface = surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1; //1 unless stereoscopic 3D application.
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //post-processing : may use a value like VK_IMAGE_USAGE_TRANSFER_DST_BIT

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
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

		if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			COMPHILOG_CORE_ERROR("failed to create swap chain!");
		}

		COMPHILOG_CORE_INFO("SwapChain created Successfully!");

		vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

	}

	void GraphicsContext::createImageViews() {
		swapChainImageViews.resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];

			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; //1D textures, 2D textures, 3D textures and cube maps.
			createInfo.format = swapChainImageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; //defaultChannelMapping

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			//If you were working on a stereographic 3D application, then you would create a swap chain with multiple layers. 
			//You could then create multiple image views for each image 
			//representing the views for the left and right eyes by accessing different layers.

			if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
				COMPHILOG_CORE_FATAL("failed to create image view! {0}", i);
			}
			COMPHILOG_CORE_INFO("created image view! {0} successfully!", i);
		}

	}

#pragma endregion

	void GraphicsContext::CleanUp()
	{
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}

		COMPHILOG_CORE_INFO("vkDestroy Destroy Swapchain:");
		vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy Logical Device:");
		vkDestroyDevice(logicalDevice, nullptr);

#ifndef NDEBUG
		COMPHILOG_CORE_INFO("vkDestroy Destroy Debug Utils:");
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif //!NDEBUG

		COMPHILOG_CORE_INFO("vkDestroy Surface:");
		vkDestroySurfaceKHR(instance, surface, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Instance:");
		vkDestroyInstance(instance, nullptr);

		COMPHILOG_CORE_INFO("Vulkan GraphicsContext Cleaned Up!");
	}

	void GraphicsContext::Draw()
	{
		
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		
	}

	void GraphicsContext::SwapBuffers()
	{
		//glfwSwapBuffers(m_WindowHandle);
	}
}