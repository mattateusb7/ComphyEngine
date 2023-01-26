#pragma once
#include "../../Renderer/IGraphicsContext.h"
#include "GraphicsHandler.h"
#include "Initialization/GraphicsInstance.h"

namespace Comphi::Vulkan {

	class GraphicsContext : public IGraphicsContext //AKA Renderer
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

		Time FrameTime; //TODO: Debug ?
		MultiScene* scenes;

	protected:
		bool _framebufferResized = false;
		void createSyncObjects();
		void createCommandBuffers();
		void updateSceneLoop();
	};

}