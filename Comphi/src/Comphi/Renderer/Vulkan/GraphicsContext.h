#pragma once
#include "Comphi/Renderer/IGraphicsContext.h"

#include "GraphicsHandler.h"

#include "Objects/VertexBuffer.h"
#include "Objects/IndexBuffer.h"
#include "Objects/UniformBuffer.h"
#include "Objects/ImageBufer.h"
#include "Objects/ImageView.h"
#include "Objects/ShaderProgram.h"

#include "Initialization/GraphicsInstance.h"
#include "Initialization/GraphicsPipeline.h"
#include "Initialization/RenderPass.h"
#include "Initialization/SwapChain.h"
#include "Initialization/DescriptorPool.h"
#include "Initialization/CommandPool.h"
#include "Initialization/SyncObjects.h"

#include "Comphi/Core/API/MeshObject.h" //TODO: TEMP

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

		bool framebufferResized = false;
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void updateUniformBuffer(uint32_t currentImage);

	public:

		std::unique_ptr<GraphicsInstance> graphicsInstance;
		std::unique_ptr<SwapChain> swapchain;
		std::unique_ptr<CommandPool> commandPool;
		std::unique_ptr<GraphicsPipeline> graphicsPipeline;
		std::unique_ptr<SyncObjects> syncObjects;

		// TODO : Shader Program/pipeline
		//Sahders //needs rework 
		//(dynamic binding on the fly compatible with renderpass & descriptorpool ?)
		bool BindProgram(IShaderProgram& shaderProgram);
		bool UnbindProgram(IShaderProgram& shaderProgram);
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages; //aligned with renderpass & descriptorpool
		std::vector<ShaderProgram*>shaderPrograms;
		MeshObject obj1;
	};

}