#pragma once
#include "Comphi/Core/Core.h"
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

			bool isComplete() {
				return graphicsFamily.has_value();
			}
		};

		void createVKInstance();
		std::vector<const char*> getRequiredGLFWExtensions();
		bool checkGLFWRequiredInstanceExtensions(const char**& glfwExtensions, uint32_t& glfwExtensionCount);
		
		void createSurface();

		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);

		void createLogicalDevice();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

#ifndef NDEBUG
		bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
		void setupDebugMessenger();
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
#endif

	private:

#ifndef NDEBUG
		VkDebugUtilsMessengerEXT debugMessenger;
		std::vector<const char*> validationLayers;
#endif
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		VkQueue graphicsQueue;

		VkSurfaceKHR surface;
		GLFWwindow* m_WindowHandle;
		
		std::unique_ptr<IVertexBuffer> vao;
		std::unique_ptr<IShaderProgram> vertexShader;
		std::unique_ptr<IShaderProgram> fragmentShader;
		std::unique_ptr<IShaderPipeline> shaderPipe;
	};
}