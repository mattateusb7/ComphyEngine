#pragma once
#include "Comphi/Renderer/IGraphicsContext.h"
#include "Comphi/Renderer/Vulkan/GraphicsHandler.h"
#include "Comphi/Renderer/Vulkan/GraphicsInstance.h"
#include "Comphi/Utils/Time.h"

namespace Comphi::Vulkan {

	class GraphicsContext : public IGraphicsContext //AKA Renderer
	{
	public:
		GraphicsContext(GLFWwindow& windowHandle);
		virtual void Init() override;
		virtual void SetScenes(SceneGraph& scenes) override;
		virtual void Draw() override;
		virtual void ResizeWindow(uint x, uint y) override;
		virtual void ResizeFramebuffer(uint x, uint y) override;
		virtual void CleanUp() override;

		std::unique_ptr<GraphicsInstance> graphicsInstance;

		Time FrameTime; //TODO: Debug ?
		SceneGraph* scenes;

	protected:
		bool _framebufferResized = false;
		void createSyncObjects();
		void createCommandBuffers();
		void updateSceneLoop();
	};

}