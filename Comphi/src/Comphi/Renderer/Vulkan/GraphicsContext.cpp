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
		//TODO : Shader stages -> Move outside of Graphics context
		Windows::FileRef vert = Windows::FileRef("shaders\\vert.spv");
		Windows::FileRef frag = Windows::FileRef("shaders\\frag.spv");

		auto vertShader = ShaderProgram(ShaderType::VertexShader, vert, *GraphicsHandler::get()->logicalDevice);
		auto fragShader = ShaderProgram(ShaderType::FragmentShader, frag, *GraphicsHandler::get()->logicalDevice);

		BindProgram(vertShader);
		BindProgram(fragShader);

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

		/*
		* Driver developers recommend that you also store multiple buffers, like the vertex and index buffer, into a single VkBuffer
		* (DrawBuffer or maybe batchDrawBuffer/multipleObjs)
		* and use offsets in commands like vkCmdBindVertexBuffers.
		* The advantage is that your data is more cache friendly in that case, because it's closer together.
		*/

		obj1.initialize("models/viking_room.obj", "textures/viking_room.png");
		//obj1.initialize("models/BLEPOSPACE.obj", "textures/texture.jpg");
		//obj1.initialize("models/sponza.obj", "textures/texture.jpg");
		//obj1.initialize(cubeVx, CubeIx, "textures/texture_2.jpg");

		int end = 1;
		// ------------------------------------------------------------
		obj1.initUBO(swapchain->MAX_FRAMES_IN_FLIGHT);
		swapchain->renderPass->descriptorPool->updateDescriptorSet(obj1, swapchain->MAX_FRAMES_IN_FLIGHT);
		graphicsPipeline = std::make_unique<GraphicsPipeline>(*swapchain->renderPass.get(), shaderStages);

		UnbindProgram(vertShader);
		UnbindProgram(fragShader);

		syncObjects = std::make_unique<SyncObjects>(swapchain->MAX_FRAMES_IN_FLIGHT);
	}

#pragma region //DEBUG!

	bool GraphicsContext::BindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram); //Interfaces Need Rework

		switch (shaderProgram.GetType())
		{
		case (uint)Comphi::ShaderType::VertexShader: {
			//VERTEX
			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = _shaderProgram->shaderModule;
			vertShaderStageInfo.pName = "main";
			vertShaderStageInfo.pSpecializationInfo = nullptr;
			shaderStages.push_back(vertShaderStageInfo);
			break;
		}
		case (uint)Comphi::ShaderType::FragmentShader: {
			//FRAGMENT
			VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = _shaderProgram->shaderModule;
			fragShaderStageInfo.pName = "main";
			shaderStages.push_back(fragShaderStageInfo);
			break;
		}
		default:
			break;
		}

		shaderPrograms.push_back(_shaderProgram);

		return true;
	}

	bool GraphicsContext::UnbindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram);

		auto it_shaderProgram = std::find(shaderPrograms.begin(), shaderPrograms.end(), _shaderProgram);
		if (it_shaderProgram != shaderPrograms.end()) {
			COMPHILOG_CORE_INFO("Destroyed ShaderModule!");
			vkDestroyShaderModule(*GraphicsHandler::get()->logicalDevice, (*it_shaderProgram)->shaderModule, nullptr);
			shaderPrograms.erase(it_shaderProgram);
			return true;
		}
		return false;
	}

	void GraphicsContext::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		//StartRecordingCommandBuffer
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to begin recording command buffer!");
			throw std::runtime_error("failed to begin recording command buffer!");
			return;
		}

		//graphics pipeline & render attachment(framebuffer/img) selection 
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapchain->renderPass->renderPassObj;
		renderPassInfo.framebuffer = swapchain->swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapchain->swapChainExtent;

		std::array<VkClearValue, 2> clearValues{}; //same order as attachments
		clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); 
		{//begin render pass

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineObj);

			//Bind VertexBuffers @0
			VkBuffer vertexBuffers[] = { obj1.vertices->bufferObj };
			VkDeviceSize offsets[] = { 0 }; //batch render
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			//Bind IndexBuffers @1
			vkCmdBindIndexBuffer(commandBuffer, obj1.indices->bufferObj, 0, VK_INDEX_TYPE_UINT32);

			//dynamic VIEWPORT/SCISSOR SETUP
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(swapchain->swapChainExtent.width);
			viewport.height = static_cast<float>(swapchain->swapChainExtent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = swapchain->swapChainExtent;
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			//BindIndex UniformBuffers @2
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, &swapchain->renderPass->descriptorPool->descriptorSets[swapchain->currentFrame], 0, nullptr);

			//DRAW COMMAND
			vkCmdDrawIndexed(commandBuffer, obj1.indices->indexCount, 1, 0, 0, 0);
			//vkCmdDraw(commandBuffer, this->vertexBuffers[0]->vertexCount, 1, 0, 0);

		}//end render pass
		
		vkCmdEndRenderPass(commandBuffer);

		//EndRecordingCommandBuffer
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to record command buffer!");
			throw std::runtime_error("failed to record command buffer!");
			return;
		}

	}

	void GraphicsContext::updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float Time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		//glm::abs(glm::sin(Time))

		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), /*glm::sin(Time)*/ Time * glm::radians(45.0f), glm::vec3(0, 0, 1.0)); /*glm::vec3(glm::sin(Time)0.0f, 0.5f, 1.0f*/

		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		ubo.proj = glm::perspective(glm::radians(45.0f), swapchain->swapChainExtent.width / (float)swapchain->swapChainExtent.height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		void* data;

		vkMapMemory(graphicsInstance->logicalDevice, obj1.ubos[currentImage]->bufferMemory, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(graphicsInstance->logicalDevice, obj1.ubos[currentImage]->bufferMemory);
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
		vkResetCommandBuffer(commandPool->commandBuffers[swapchain->currentFrame], 0);
		recordCommandBuffer(commandPool->commandBuffers[swapchain->currentFrame], imageIndex);

		updateUniformBuffer(swapchain->currentFrame);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { syncObjects->imageAvailableSemaphores[swapchain->currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandPool->commandBuffers[swapchain->currentFrame];

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
		graphicsPipeline->~GraphicsPipeline();
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

	void GraphicsContext::SwapBuffers()
	{
		//glfwSwapBuffers(windowHandle);
	}
}