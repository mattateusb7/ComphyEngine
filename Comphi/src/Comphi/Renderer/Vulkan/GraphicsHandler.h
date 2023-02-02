#pragma once
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>

namespace Comphi::Vulkan {

	class DeviceHandler {
	public:
		DeviceHandler() = default;
		VkDevice logicalDevice;
		VkPhysicalDevice physicalDevice;
		void setDeviceHandler(
			const VkDevice& logicalDevice,
			const VkPhysicalDevice& physicalDevice
		) {
			this->logicalDevice =  logicalDevice;
			this->physicalDevice = physicalDevice;
		}
	};

	class QueueHandler {
	public:
		QueueHandler() = default;
		struct CommandQueueFamily {
			uint32_t index; //not a pointer
			VkCommandPool commandPool;
			VkQueue queue;
		};
		CommandQueueFamily transferQueueFamily;
		CommandQueueFamily graphicsQueueFamily;
		//CommandQueueFamily presentQueueFamily;
		void setCommandQueues(
			const uint32_t transferQueueFamilyIndex,
			const VkQueue transferQueue,
			const uint32_t graphicsQueueFamilyIndex,
			const VkQueue graphicsQueue
		) {
			this->transferQueueFamily.index = transferQueueFamilyIndex;
			this->transferQueueFamily.queue = transferQueue;

			this->graphicsQueueFamily.index = graphicsQueueFamilyIndex;
			this->graphicsQueueFamily.queue = graphicsQueue;
		}

		void setCommandPools(
			const VkCommandPool transferCommandPool,
			const VkCommandPool graphicsCommandPool
		) {
			this->transferQueueFamily.commandPool = transferCommandPool;
			this->graphicsQueueFamily.commandPool = graphicsCommandPool;
		}
	};

	class WindowHandler {
	public:
		WindowHandler() = default;
		VkSurfaceKHR surface;
		GLFWwindow* windowHandle;
		void setWindowHandler(
			GLFWwindow* windowHandle,
			const VkSurfaceKHR& surface
		) {
			this->surface = surface;
			this->windowHandle = windowHandle;
		}

	};

	class SwapchainHandler {
	public:
		SwapchainHandler() = default;
		int* MAX_FRAMES_IN_FLIGHT;
		VkRenderPass* renderPass;
		VkExtent2D* swapChainExtent;
		void setSwapchainHandler(
			VkRenderPass& renderPass,
			int& MAX_FRAMES_IN_FLIGHT,
			VkExtent2D& swapChainExtent

		) {
			this->renderPass = &renderPass;
			this->MAX_FRAMES_IN_FLIGHT = &MAX_FRAMES_IN_FLIGHT;
			this->swapChainExtent = &swapChainExtent;
		}
	};	
	
	//TODO: evaluate if possible to make instanced per Vulkan GraphicsContext (MultiGraphicsContext Handler)?
	class GraphicsHandler : public DeviceHandler, public QueueHandler , public WindowHandler, public SwapchainHandler {
	public:
		GraphicsHandler() = default;
		static GraphicsHandler* get();

		bool isInUse = true;
		void DeleteStatic();
		~GraphicsHandler();

	};

}
