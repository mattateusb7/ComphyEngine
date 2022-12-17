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

	enum CommandQueueOperation { TransferCommand, GraphicsCommand };

	struct CommandBuffer {
		CommandQueueOperation op = TransferCommand;
		VkCommandBuffer buffer;
	};

	//TODO: evaluate if possible to make instanced per Vulkan GraphicsContext (MultiGraphicsContext Handler)?
	class GraphicsHandler : public DeviceHandler, public QueueHandler , public WindowHandler, public SwapchainHandler {
	public:
		GraphicsHandler() = default;
		static GraphicsHandler* get();

		//CommandBuffers
		static CommandBuffer beginCommandBuffer(CommandQueueOperation op);
		static void endCommandBuffer(CommandBuffer& commandBuffer);

		//MemoryHandling
		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static void copyBufferTo(VkBuffer& srcBuffer, VkBuffer& dstBuffer, uint copySize);

		bool isInUse = true;
		void DeleteStatic();
		~GraphicsHandler();

	protected:
		static VkCommandPool getCommandPool(CommandQueueOperation& op);
		static VkQueue getCommandQueue(CommandQueueOperation& op);

	};
}
