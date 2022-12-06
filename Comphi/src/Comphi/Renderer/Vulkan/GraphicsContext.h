#pragma once
#include "../../Renderer/IGraphicsContext.h"
#include "GraphicsHandler.h"
#include "Initialization/GraphicsInstance.h"
#include "Initialization/SwapChain.h"
#include "Initialization/CommandPool.h"

namespace Comphi::Vulkan {

	class GraphicsContext : public IGraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow& windowHandle);
		virtual void Init() override;
		virtual void SetScenes(MultiScene& scenes) override;
		virtual void Draw() override;
		virtual void ResizeWindow(uint x, uint y) override;
		virtual void ResizeFramebuffer(uint x, uint y) override;
		virtual void CleanUp() override;

		std::unique_ptr<GraphicsInstance> graphicsInstance;
		std::unique_ptr<SwapChain> swapchain;
		std::unique_ptr<CommandPool> commandPool;

		Time FrameTime; //TODO: Debug ?
		MultiScene* scenes;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

	protected:
		bool framebufferResized = false;
		void createSyncObjects();
		void updateSceneLoop();
	};

}