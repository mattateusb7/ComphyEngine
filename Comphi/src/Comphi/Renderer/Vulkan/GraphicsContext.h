#pragma once
#include "Comphi/Renderer/IGraphicsContext.h"

#include "GraphicsHandler.h"

#include "Objects/VertexBuffer.h"
#include "Objects/IndexBuffer.h"
#include "Objects/UniformBuffer.h"
#include "Objects/ImageBufer.h"
#include "Objects/ImageView.h"
#include "Objects/SwapChain.h"
#include "Objects/ShaderProgram.h"

#include "GraphicsPipeline.h"

#include "Comphi/Core/API/MeshObject.h"

namespace Comphi::Vulkan {

	class GraphicsContext : public IGraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow& windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Draw() override;
		virtual void ResizeWindow(uint x, uint y) override;
		virtual void ResizeFramebuffer(uint x, uint y) override;
		virtual void CleanUp() override;

	protected:

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			std::optional<uint32_t> transferFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
			}
		};
		GraphicsContext::QueueFamilyIndices queueFamilyIndices;

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
		void recreateSwapChain();
		void cleanupSwapChain();

		//Graphics pipeline
		void createGraphicsPipeline();
		//VkShaderModule createShaderModule(const std::vector<char>& code);
		
		//RenderPass
		void createRenderPass();

		//Framebuffers
		void createFramebuffers();

		void createDebugBuffers();

		//command Pool/Buffer
		void createCommandPools();
		void createCommandBuffers();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void createSyncObjects();

		void createDescriptorPool();
		void createDescriptorSet();

		void updateUniformBuffer(uint32_t currentImage);

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

	public:

		VkInstance instance;

#ifndef NDEBUG
		VkDebugUtilsMessengerEXT debugMessenger;
#endif //!NDEBUG

		//Move statics to Graphics handler?
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkSurfaceKHR surface;
		GLFWwindow* m_WindowHandle;

		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkQueue transferQueue;

		VkCommandPool graphicsCommandPool;
		VkCommandPool transferCommandPool;

		std::unique_ptr<GraphicsPipeline> graphicsPipeline;
		std::unique_ptr<SwapChain> swapchain;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		const int MAX_FRAMES_IN_FLIGHT = 2; //double-buffering
		uint32_t currentFrame = 0;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		bool framebufferResized = false;

		MeshObject obj1;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		
	};
}