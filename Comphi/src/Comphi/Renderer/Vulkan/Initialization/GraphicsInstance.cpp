#include "cphipch.h"
#include "GraphicsInstance.h"
#include <optional>
#include <set>

namespace Comphi::Vulkan {

#pragma region Vulkan_Instance

	GraphicsInstance::GraphicsInstance()
	{
		createVKInstance();
#ifndef NDEBUG
		setupDebugMessenger();
#endif //!NDEBUG
		createSurface();
		GraphicsHandler::get()->setWindowHandler(GraphicsHandler::get()->windowHandle, surface);

		pickPhysicalDevice();
		createLogicalDevices();

		GraphicsHandler::get()->setDeviceHandler(logicalDevice, physicalDevice);

		GraphicsHandler::get()->setCommandQueues(
			queueFamilyIndices.transferFamily.value(),
			transferQueue,
			queueFamilyIndices.graphicsFamily.value(),
			graphicsQueue
		);
	}

	GraphicsInstance::~GraphicsInstance()
	{

		COMPHILOG_CORE_INFO("vkDestroy Surface");
		vkDestroySurfaceKHR(instance, surface, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy Logical Device");
		vkDestroyDevice(logicalDevice, nullptr);

#ifndef NDEBUG
		COMPHILOG_CORE_INFO("vkDestroy Destroy Debug Utils");
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif //!NDEBUG

		COMPHILOG_CORE_INFO("vkDestroy Instance");
		vkDestroyInstance(instance, nullptr);

		COMPHILOG_CORE_INFO("Vulkan GraphicsContext Cleaned Up!");
	}

	void GraphicsInstance::createSurface() {
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(GraphicsHandler::get()->windowHandle);
		createInfo.hinstance = GetModuleHandle(nullptr);

		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("Failed to create window surface!");
			throw std::runtime_error("Failed to create window surface!");
		}

		if (glfwCreateWindowSurface(instance, GraphicsHandler::get()->windowHandle, nullptr, &surface) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("Failed to create window surface!");
			throw std::runtime_error("Failed to create window surface!");
		}


		COMPHILOG_CORE_INFO("vk_surface window creation successful!");
	}

	void GraphicsInstance::createVKInstance()
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
#ifdef NDEBUG
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#else
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		if (!GraphicsInstance::checkValidationLayerSupport(validationLayers)) {
			COMPHILOG_CORE_FATAL("validation layers requested, but not available!");
			throw std::runtime_error("validation layers requested, but not available!");
			return;
		}

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

#endif //!NDEBUG

		if (extensions.size() > 0) {
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();
		}

		if (&createInfo == nullptr || &instance == nullptr) {
			COMPHILOG_CORE_FATAL("Null pointer passed to vkCreateInstance!");
			throw std::runtime_error("Null pointer passed to vkCreateInstance!");
			return;
		}
		else if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create vkinstance!");
			throw std::runtime_error("failed to create vkinstance!");
			return;
		}
		COMPHILOG_CORE_INFO("vk instance creation successful!");
	}

#ifndef NDEBUG
	bool GraphicsInstance::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {

		COMPHILOG_CORE_TRACE("Requesting Validation Layers");

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

	void GraphicsInstance::setupDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to set up debug messenger!");
			throw std::runtime_error("failed to set up debug messenger!");
			return;
		}
		COMPHILOG_CORE_INFO("DebugMessenger setup successful!");
	}

	void GraphicsInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsInstance::debugCallback(
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
			throw std::runtime_error(pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}

	VkResult GraphicsInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void GraphicsInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

#endif //!NDEBUG

	std::vector<const char*> GraphicsInstance::getRequiredGLFWExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		if (!checkGLFWRequiredInstanceExtensions(glfwExtensions, glfwExtensionCount)) {
			return std::vector<const char*>();
		}

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return extensions;
	}

	bool GraphicsInstance::checkGLFWRequiredInstanceExtensions(const char**& glfwExtensions, uint32_t& glfwExtensionCount) {

		COMPHILOG_CORE_TRACE("Requesting GLFW RequiredInstanceExtensions");

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
				throw std::runtime_error("GLFW REQUIRED EXTENSION MISSING");
				return false;
			}
		}
		return true;
	}
#pragma endregion

#pragma region Phisical_Device

	void GraphicsInstance::pickPhysicalDevice() {
		physicalDevice = VK_NULL_HANDLE;

		COMPHILOG_CORE_TRACE("Queue PhysicalDevices...");
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			COMPHILOG_CORE_FATAL("failed to find GPUs with Vulkan support!");
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
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
			throw std::runtime_error("failed to find a suitable GPU!");
			return;
		}
		COMPHILOG_CORE_INFO("PhysicalDevice setup successful!");

	}

	GraphicsInstance::QueueFamilyIndices GraphicsInstance::findQueueFamilies(VkPhysicalDevice device) {

		//COMPHILOG_CORE_TRACE("Requesting QueueFamilies...");
		QueueFamilyIndices indices;
		// Assign index to queue families that could be found

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		if (queueFamilyCount == 0) {
			COMPHILOG_CORE_FATAL("failed to queueFamilies for device!");
			throw std::runtime_error("failed to queueFamilies for device");
			return indices;
		}

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int graphicsQueueflags = (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT);
		int transferQueueflags = (VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT);

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport) {
				indices.presentFamily = i;
			}
			if ((queueFamily.queueFlags & graphicsQueueflags) == graphicsQueueflags /*&& (queueFamily.queueFlags & transferQueueflags) == 0*/) {
				indices.graphicsFamily = i;
			}
			if ((queueFamily.queueFlags & transferQueueflags) == transferQueueflags && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) {
				//VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT, but not the VK_QUEUE_GRAPHICS_BIT
				indices.transferFamily = i;
			}
			if (indices.isComplete()) {
				COMPHILOG_CORE_INFO("queueFamily found!");
				break;
			}
			i++;
		}

		return indices;
	}

	bool GraphicsInstance::isDeviceSuitable(VkPhysicalDevice device) {
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

		queueFamilyIndices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(device, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures; //Optimize fetch in device ClassObject
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return queueFamilyIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool GraphicsInstance::checkDeviceExtensionSupport(VkPhysicalDevice device) {
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

#pragma endregion

#pragma region Logical_Device_Queues
	void GraphicsInstance::createLogicalDevices() {
		if (physicalDevice == VK_NULL_HANDLE) return;

		COMPHILOG_CORE_TRACE("Creating Logical Device...");

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value(), queueFamilyIndices.transferFamily.value() };

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
		deviceFeatures.samplerAnisotropy = VK_TRUE;

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
			throw std::runtime_error("failed to create logical device");
		}
		COMPHILOG_CORE_INFO("Logical Device creation successful!");

		vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
		COMPHILOG_CORE_INFO("Graphics Queue request successful!");

		vkGetDeviceQueue(logicalDevice, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
		COMPHILOG_CORE_INFO("Present Queue request successful!");

		vkGetDeviceQueue(logicalDevice, queueFamilyIndices.transferFamily.value(), 0, &transferQueue);
		COMPHILOG_CORE_INFO("Transfer Queue request successful!");
	}

#pragma endregion
}