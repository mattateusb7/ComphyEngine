#include "cphipch.h"
#include "GraphicsContext.h"


#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace Comphi::Vulkan {

	GraphicsContext::GraphicsContext(GLFWwindow& windowHandle)
	{
		GraphicsHandler::get()->windowHandle = &windowHandle;
		COMPHILOG_CORE_ASSERT(&windowHandle, "Window Handle is NULL!");
	}

/*INFO
//! VULKAN Guide: https://vulkan-tutorial.com/
//! VULKAN Guide2: https://vkguide.dev/
//! VULKAN Map	https://github.com/David-DiGioia/vulkan-diagrams
//! VULKAN SPIR Compile : https://www.khronos.org/spir/
*/

	void GraphicsContext::Init()
	{
		graphicsInstance = std::make_unique<GraphicsInstance>();
		commandPool = std::make_unique<CommandPool>();
		swapchain = std::make_unique<SwapChain>();
		commandPool->createCommandBuffers(swapchain->MAX_FRAMES_IN_FLIGHT);

		// ------------------------------------------------------------
		//					GRAPHICS PIPELINE 
		//TODO : Shader/Texture/Material initialization stage -> Move outside of Graphics context

		descriptorPool = MakeDescriptorPoolInstance(); //TODO: Single Layout for now

		MaterialProperties materialProperties;
		materialProperties.descriptorPool = descriptorPool.get();

		std::string Texfile = "textures/viking_room.png";
		texture1 = MakeTextureInstance(Texfile);

		std::vector<Texture*> textures = { texture1.get()};
		materialProperties.textures = textures;

		vert = Windows::FileRef("shaders\\vert.spv");
		frag = Windows::FileRef("shaders\\frag.spv");
		vertShader = MakeShaderProgramInstance(ShaderType::VertexShader, vert);
		fragShader = MakeShaderProgramInstance(ShaderType::FragmentShader, frag);

		std::vector<IShaderProgram*> shaders = { &*vertShader , &*fragShader };
		materialProperties.shaders = shaders;

		//shared Material Instance
		Albedo1 = MakeMaterialInstance(materialProperties);


		// ------------------------------------------------------------
		//					TEST OBJECTS PIPELINE 
		const VertexArray vertices = {
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};

		const IndexArray indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		VertexArray cubeVx = {
			{{ 0.5f, 0.5f, 0.5f} , {1.0f, 1.0f, 1.0f} , {1.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.5f} , {1.0f, 1.0f, 0.0f} , {0.0f, 0.0f}},
			{{-0.5f,-0.5f, 0.5f} , {1.0f, 0.0f, 0.0f} , {0.0f, 1.0f}},
			{{ 0.5f,-0.5f, 0.5f} , {1.0f, 0.0f, 1.0f} , {1.0f, 1.0f}},
			{{ 0.5f,-0.5f,-0.5f} , {0.0f, 0.0f, 1.0f} , {1.0f, 0.0f}},
			{{ 0.5f, 0.5f,-0.5f} , {0.0f, 1.0f, 1.0f} , {0.0f, 0.0f}},
			{{-0.5f, 0.5f,-0.5f} , {0.0f, 1.0f, 0.0f} , {0.0f, 1.0f}},
			{{-0.5f,-0.5f,-0.5f} , {0.0f, 0.0f, 0.0f} , {1.0f, 1.0f}}
		};

		IndexArray CubeIx = {
			0, 1, 2,   2, 3, 0,   // v0-v1-v2, v2-v3-v0 (front)
			0, 3, 4,   4, 5, 0,   // v0-v3-v4, v4-v5-v0 (right)
			0, 5, 6,   6, 1, 0,   // v0-v5-v6, v6-v1-v0 (top)
			1, 6, 7,   7, 2, 1,   // v1-v6-v7, v7-v2-v1 (left)
			7, 4, 3,   3, 2, 7,   // v7-v4-v3, v3-v2-v7 (bottom)
			4, 7, 6,   6, 5, 4    // v4-v7-v6, v6-v5-v4 (back)
		};

		modelMesh = Windows::FileRef("models/viking_room.obj");
		meshObj1 = MakeMeshInstance(modelMesh, *Albedo1.get());
		gameObj1 = MakeGameObjectInstance(meshObj1);

		camObj1 = MakeCameraInstance();
		camObj1->transform.position = glm::vec3(0.0f, 4.0f, 0.0f);

		// ------------------------------------------------------------
	
		syncObjects = std::make_unique<SyncObjects>(swapchain->MAX_FRAMES_IN_FLIGHT);
	}

#pragma region //DEBUG!

	void GraphicsContext::updateUniformBuffers(uint32_t currentImage) {

		//TODO: Move this vvv to a TIME Class
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float Time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		// ^^^

		gameObj1->transform.position = glm::vec3(0, 0, glm::sin(Time));
		gameObj1->transform.setEulerAngles(glm::vec3(0.0f, 0.0f, 45.0f) * Time);
		
		camObj1->transform.lookAt(gameObj1->transform.position);

		//Update MVP_UBOs per GameObject
		UniformBufferObject ubo{};
		ubo.model = gameObj1->transform.getModelMatrix();
		ubo.view = camObj1->getViewMatrix();
		ubo.proj = camObj1->getProjectionMatrix();

		gameObj1->updateMVP(ubo,currentImage);

	}
#pragma endregion

	void GraphicsContext::Draw()
	{
		//Wait for the previous frame to finish
		//Acquire an image from the swap chain
		//Record a command buffer which draws the scene onto that image
		//Submit the recorded command buffer
		//Present the swap chain image

		vkWaitForFences(graphicsInstance->logicalDevice, 1, &syncObjects->inFlightFences[swapchain->currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(graphicsInstance->logicalDevice, swapchain->swapChainObj, UINT64_MAX, syncObjects->imageAvailableSemaphores[swapchain->currentFrame], VK_NULL_HANDLE, &imageIndex);
		if (result != VK_SUCCESS) {
			COMPHILOG_CORE_ERROR("failed to acquireNextImage!");
			//throw std::runtime_error("failed to acquireNextImage!");
		}
		else {
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				swapchain->recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				COMPHILOG_CORE_ERROR("failed to acquire swap chain image!");
				throw std::runtime_error("failed to acquire swap chain image!");
			}
		}

		// Only reset the fence if we are submitting work
		vkResetFences(graphicsInstance->logicalDevice, 1, &syncObjects->inFlightFences[swapchain->currentFrame]);

		//vkResetCommandPool(graphicsInstance->logicalDevice, commandPool->graphicsCommandPool,0); 
		//if you are making multiple command buffers from one pool, resetting the pool will be quicker.
		vkResetCommandBuffer(commandPool->graphicsCommandBuffers[swapchain->currentFrame], 0);

		//SEND render COMMANDS TO GPU
		swapchain->recordCommandBuffer(commandPool->graphicsCommandBuffers[swapchain->currentFrame], *meshObj1.get(), imageIndex);

		updateUniformBuffers(swapchain->currentFrame);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { syncObjects->imageAvailableSemaphores[swapchain->currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandPool->graphicsCommandBuffers[swapchain->currentFrame];

		VkSemaphore signalSemaphores[] = { syncObjects->renderFinishedSemaphores[swapchain->currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsInstance->graphicsQueue, 1, &submitInfo, syncObjects->inFlightFences[swapchain->currentFrame]) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to submit draw command buffer!");
			throw std::runtime_error("failed to submit draw command buffer!");
			return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		
		VkSwapchainKHR swapChains[] = { swapchain->swapChainObj };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional error handling

		result = vkQueuePresentKHR(graphicsInstance->presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			swapchain->recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to present swap chain image!");
			throw std::runtime_error("failed to present swap chain image!");
			return;
		}

		swapchain->incrementSwapChainFrame();
	}

	void GraphicsContext::CleanUp()
	{
		vkDeviceWaitIdle(graphicsInstance->logicalDevice);

		swapchain->~SwapChain();
		commandPool->~CommandPool();
		//graphicsPipeline->~GraphicsPipeline();
		syncObjects->~SyncObjects();
		graphicsInstance->~GraphicsInstance();
		GraphicsHandler::get()->DeleteStatic();
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		ResizeFramebuffer(x, y);
	}

	void GraphicsContext::ResizeFramebuffer(uint x, uint y)
	{
		framebufferResized = true;
	}
}