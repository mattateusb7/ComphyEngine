#pragma once
#include "Comphi/Renderer/IGraphicsContext.h"

#include "GraphicsHandler.h"

#include "Initialization/GraphicsInstance.h"
#include "Initialization/SwapChain.h"
#include "Initialization/CommandPool.h"
#include "Initialization/SyncObjects.h"

#include "Comphi/Core/API/Camera.h" //TODO: TEMP debug

namespace Comphi::Vulkan {

	class GraphicsContext : public IGraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow& windowHandle);
		virtual void Init() override;
		virtual void Draw() override;
		virtual void ResizeWindow(uint x, uint y) override;
		virtual void ResizeFramebuffer(uint x, uint y) override;
		virtual void CleanUp() override;

	protected:
		bool framebufferResized = false;
		//Debug ?
		void updateUniformBuffers(uint32_t currentImage);
	public:

		std::unique_ptr<GraphicsInstance> graphicsInstance;
		std::unique_ptr<SwapChain> swapchain;
		std::unique_ptr<CommandPool> commandPool;
		std::unique_ptr<SyncObjects> syncObjects;

		//TODO: remove DEBUG objs
		DescriptorPoolInstance descriptorPool;
		MeshInstance meshObj1;
		TextureInstance texture1;
		MaterialInstance Albedo1;
		ShaderProgramInstance vertShader;
		ShaderProgramInstance fragShader;
		GameObjectInstance gameObj1;
		CameraInstance camObj1;
		Windows::FileRef vert;
		Windows::FileRef frag;
		Windows::FileRef modelMesh;
	};

}