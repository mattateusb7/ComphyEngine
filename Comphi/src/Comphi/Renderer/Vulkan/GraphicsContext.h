#pragma once
#include "Comphi/Core/Core.h"
#include "Comphi/Platform/Windows/FileRef.h"
#include "Comphi/Events/Event.h"
#include "Comphi/Renderer/IGraphicsContext.h"
#include "Comphi/Renderer/GraphicsAPI.h"

#include <GLFW/glfw3.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_win32.h>

namespace Comphi::Vulkan {
	class GraphicsContext : public IGraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow& windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Draw() override;
		virtual void ResizeWindow(uint x, uint y) override;
		virtual void CleanUp() override;
	
	protected:

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifndef NDEBUG
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
#endif //!NDEBUG

		//VKInstance
		void createVKInstance();
		std::vector<const char*> getRequiredGLFWExtensions();
		bool checkGLFWRequiredInstanceExtensions(const char**& glfwExtensions, uint32_t& glfwExtensionCount);
		
		//Surface
		void createSurface();

		//Physical Device
		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		//Logical Device
		void createLogicalDevice();

		//indexedQueues
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		//SwapChain
		void createSwapChain();
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		//ImageViews
		void createImageViews();

		//Graphics pipeline
		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		
		//RenderPass
		void createRenderPass();

#ifndef NDEBUG
		//Validation Layers
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

	private:

#ifndef NDEBUG
		VkDebugUtilsMessengerEXT debugMessenger;
#endif //!NDEBUG
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImage> swapChainImages;

		std::vector<VkImageView> swapChainImageViews;
		GLFWwindow* m_WindowHandle;

		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		//std::unique_ptr<IVertexBuffer> vao;
		std::unique_ptr<IShaderProgram> vertexShader;
		std::unique_ptr<IShaderProgram> fragmentShader;
		std::unique_ptr<IShaderPipeline> shaderPipe;
	};
}