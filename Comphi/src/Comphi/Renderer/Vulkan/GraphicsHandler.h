#pragma once
#include "Common.h"

namespace Comphi::Vulkan {

	class DeviceHandler {
	public:
		DeviceHandler() = default;
		std::shared_ptr<VkDevice> logicalDevice;
		std::shared_ptr<VkPhysicalDevice> physicalDevice;
		void setDeviceHandler(
			const VkDevice& logicalDevice,
			const VkPhysicalDevice& physicalDevice
		) {
			this->logicalDevice = std::make_shared<VkDevice>(logicalDevice);
			this->physicalDevice = std::make_shared<VkPhysicalDevice>(physicalDevice);
		}
	};

	class QueueHandler {
	public:
		QueueHandler() = default;
		struct CommandQueueFamily {
			std::shared_ptr<uint32_t> index;
			std::shared_ptr<VkCommandPool> commandPool;
			std::shared_ptr<VkQueue> queue;
		};
		CommandQueueFamily transferQueueFamily;
		CommandQueueFamily graphicsQueueFamily;
		void setCommandQueues(
			const uint32_t& transferQueueFamilyIndex,
			const VkQueue& transferQueue,
			const uint32_t& graphicsQueueFamilyIndex,
			const VkQueue& graphicsQueue
		) {
			this->transferQueueFamily.index = std::make_shared<uint32_t>(transferQueueFamilyIndex);
			this->transferQueueFamily.queue = std::make_shared<VkQueue>(transferQueue);

			this->graphicsQueueFamily.index = std::make_shared<uint32_t>(graphicsQueueFamilyIndex);
			this->graphicsQueueFamily.queue = std::make_shared<VkQueue>(graphicsQueue);
		}

		void setCommandPools(
			const VkCommandPool& transferCommandPool,
			const VkCommandPool& graphicsCommandPool
		) {
			this->transferQueueFamily.commandPool = std::make_shared<VkCommandPool>(transferCommandPool);
			this->graphicsQueueFamily.commandPool = std::make_shared<VkCommandPool>(graphicsCommandPool);
		}
	};

	class WindowHandler {
	public:
		WindowHandler() = default;
		std::shared_ptr<VkSurfaceKHR> surface;
		GLFWwindow* windowHandle;
		void setWindowHandler(
			GLFWwindow* windowHandle,
			const VkSurfaceKHR& surface
		) {
			this->surface = std::make_shared<VkSurfaceKHR>(surface);
			this->windowHandle = windowHandle;
		}

	};

	//update By Context ? (isnt this class suposed to be static and generic for the whole program ? )
	//class ContextHandler {
	//public:
	//	ContextHandler() = default;
	//	std::shared_ptr<int> MAX_FRAMES_IN_FLIGHT;
	//	ContextHandler(
	//		int& MAX_FRAMES_IN_FLIGHT
	//	) {
	//		this->MAX_FRAMES_IN_FLIGHT = std::make_shared<int>(MAX_FRAMES_IN_FLIGHT);
	//	}
	//};

	enum CommandQueueOperation { TransferCommand, GraphicsCommand };

	struct CommandBuffer {
		CommandQueueOperation op = TransferCommand;
		VkCommandBuffer buffer;
	};

	class GraphicsHandler : public DeviceHandler, public QueueHandler , public WindowHandler {
	public:
		GraphicsHandler() = default;
		static GraphicsHandler* get();

		static CommandBuffer beginCommandBuffer(CommandQueueOperation op);
		static void endCommandBuffer(CommandBuffer& commandBuffer);

		bool isInUse = true;
		void DeleteStatic();
		~GraphicsHandler();
	};
}
