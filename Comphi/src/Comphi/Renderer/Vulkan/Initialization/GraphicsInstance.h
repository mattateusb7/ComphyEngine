#pragma once
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"

namespace Comphi::Vulkan {
	class GraphicsInstance
	{
	protected:

#ifndef NDEBUG

		//Validation Layers
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		void setupDebugMessenger();
		bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);
#endif //!NDEBUG

		//GLFW
		std::vector<const char*> getRequiredGLFWExtensions();
		bool checkGLFWRequiredInstanceExtensions(const char**& glfwExtensions, uint32_t& glfwExtensionCount);

		//Physical Device
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		//logical GRAPHICS QUEUES 
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			std::optional<uint32_t> transferFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
			}
		};
		QueueFamilyIndices queueFamilyIndices;
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	public:
		GraphicsInstance();
		~GraphicsInstance();

#ifndef NDEBUG
		VkDebugUtilsMessengerEXT debugMessenger;
#endif //!NDEBUG

		//Vulkan Instance
		void createVKInstance();
		VkInstance instance;

		//Surface 
		void createSurface();
		VkSurfaceKHR surface;

		//Physical Device
		void pickPhysicalDevice();
		VkPhysicalDevice physicalDevice;

		//Logical Device & GRAPHICS QUEUES
		void createLogicalDevices();
		VkDevice logicalDevice;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkQueue transferQueue;
	};
}