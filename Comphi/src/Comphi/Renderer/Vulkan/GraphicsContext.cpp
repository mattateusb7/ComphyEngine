#include "cphipch.h"
#include "GraphicsContext.h"

#include "Comphi/Renderer/GraphicsAPI.h"
#include "Comphi/Platform/Windows/FileRef.h"
#include "Comphi/Events/Event.h"

namespace Comphi::Vulkan {

	static VkInstance instance;

	GraphicsContext::GraphicsContext(GLFWwindow& windowHandle) : m_WindowHandle(&windowHandle)
	{
		COMPHILOG_CORE_ASSERT(m_WindowHandle, "Window Handle is NULL!");
	}

	GraphicsContext::~GraphicsContext()
	{
		cleanup();
	}

//!TODO: VULKANTUTO: https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Validation_layers

	void GraphicsContext::Init()
	{
		createVKInstance();
#ifndef NDEBUG
		setupDebugMessenger();
#endif
		glfwMakeContextCurrent(m_WindowHandle);

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

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		if (!GraphicsContext::checkValidationLayerSupport(validationLayers)) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

#endif

		if (extensions.size() > 0) {
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();
		}

		if (&createInfo == nullptr || &instance == nullptr) {
			throw std::runtime_error("Null pointer passed to vkCreateInstance!");
			//COMPHILOG_CORE_ASSERT(instance, "Null pointer passed to vkCreateInstance!");
			//EventThrow::Handler<Comphi::EventCategoryError>()
			// VK_ERROR_INITIALIZATION_FAILED;
		}
		else if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vkinstance!");
		}
		COMPHILOG_CORE_WARN("vk instance creation sucessful!");
	}

#ifndef NDEBUG
	bool GraphicsContext::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {

		COMPHILOG_CORE_WARN("Requesting Validation Layers:");

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					COMPHILOG_CORE_INFO(layerName);
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	void GraphicsContext::setupDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}

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
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			COMPHILOG_CORE_TRACE("VK_validation layer: {0}", pCallbackData->pMessage);
			break;
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
#endif

	void GraphicsContext::cleanup() {

#ifndef NDEBUG
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(m_WindowHandle);

		glfwTerminate();

		COMPHILOG_CORE_WARN("Vulkan GraphicsContext CleanUp!");
	}

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

		COMPHILOG_CORE_WARN("Requesting GLFW RequiredInstanceExtensions:");

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		COMPHILOG_CORE_INFO("GLFW_extensions supported : {0}", glfwExtensionCount);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		COMPHILOG_CORE_INFO("Vk_extensions supported : {0}", extensionCount);

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
				COMPHILOG_CORE_INFO("GLFW REQUIRED EXTENSION FOUND : " + *found);
				extensionsRequired.erase(found);
				continue;
			}
			/*List All Extensions*/
			//COMPHILOG_CORE_INFO(vk_ext);
		}

		if (!extensionsRequired.empty()) {
			for (std::string& ext : extensionsRequired) {
				COMPHILOG_CORE_ERROR("GLFW REQUIRED EXTENSION MISSING : " + ext);
				return false;
			}
		}
		return true;
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