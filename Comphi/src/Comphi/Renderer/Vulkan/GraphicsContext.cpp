#include "cphipch.h"
#include "GraphicsContext.h"
#include "Comphi/API/Components/Transform.h"
#include "Comphi/API/Rendering/ShaderBinding.h"
#include "Comphi/Renderer/Vulkan/Buffers/UniformBuffer.h"

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
	}

	void GraphicsContext::SetScenes(SceneGraphPtr& sceneGraph)
	{
		this->sceneGraph = sceneGraph;
	}

#pragma region //DEBUG!

	std::shared_ptr<UniformBuffer> bufferInstanceTransforms;
	void GraphicsContext::updateSceneLoop() {
		
		FrameTime.Stop();

		VkCommandBuffer& commandBuffer = graphicsInstance->swapchain->getCurrentFrameGraphicsCommandBuffer();
		graphicsInstance->swapchain->beginRenderPassCommandBuffer(commandBuffer);

		//https://computergraphics.stackexchange.com/questions/4499/how-to-change-sampler-pipeline-states-at-runtime-in-vulkan
		
		//MESH INSTANCING (WIP)
		//was trying to send dynamic storage buffer to binding point 2.2
		//	-- still trying to figure out how to setup dynamic descriptor set bindings 
		//can also try to send 4 binding points (vec4 to build model matrix in shader)
		//	-- through vertexdata binding point 1 using Instanced Rate 
	
		//BATCHED DRAW (WIP)
		//trying to use DrawIndirectIndexed to send a batched drawCall instead of multiple draw cmds
	
		//need to fix descriptor binding validation errors first
		//Traverse Render SceneGraph 
		for (const auto& cam : sceneGraph->cameras) {

			//SAME CAMERA
			glm::mat4 viewProjectionMx = cam.camera->getProjectionMatrix() * cam.transform->getViewMatrix();
			cam.camera->bufferViewProjectionMatrix->updateBufferData(&viewProjectionMx[0]);

			//dynamic VIEWPORT/SCISSOR SETUP
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(GraphicsHandler::get()->swapChainExtent->width);
			viewport.height = static_cast<float>(GraphicsHandler::get()->swapChainExtent->height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = *GraphicsHandler::get()->swapChainExtent;
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			for (const auto& batchID : sceneGraph->renderBatches) { //BATCH DRAW
				
				//DIFERENT MATERIALS 
				std::vector<VkWriteDescriptorSet> descriptorSetUpdates;
				//std::vector<VkWriteDescriptorSet> materialDescriptorSetUpdates;
				//std::vector<VkWriteDescriptorSet> entityDescriptorSetUpdates;

				//Material binding : 
				IGraphicsPipelinePtr igraphicsPipeline = batchID.material->getIPipelinePtr(); //TODO: streamline these Interface conversions later
				GraphicsPipeline* gpipeline = static_cast<GraphicsPipeline*>(igraphicsPipeline.get());
				
				//Camera DescriptorSet:
				auto projectionDescriptor = gpipeline->getDescriptorSetWrite(cam.camera->bufferViewProjectionMatrix.get(), PerMaterialInstance, 0); //<< SetID& DescriptorID need to be dynamic!
				descriptorSetUpdates.push_back(projectionDescriptor);
				
				//Material Descriptor Sets:
				MaterialInstance* currMaterialInst = batchID.materialInstance.get();
				auto texureBindings = currMaterialInst->textureBindings[PerMaterialInstance];
				auto bufferBindings = currMaterialInst->bufferBindings[PerMaterialInstance];

				//Matrial Instance Texture bindings
				for (auto& sortedBindings : texureBindings) {
					auto textures = gpipeline->getDescriptorSetWrite(sortedBindings.textures.data(), PerMaterialInstance, sortedBindings.descriptorID);
					descriptorSetUpdates.push_back(textures);
				}

				//Matrial Instance Buffer Bindings
				for (auto& sortedBindings : bufferBindings) {
					auto buffers = gpipeline->getDescriptorSetWrite(sortedBindings.buffers.data(), PerMaterialInstance, sortedBindings.descriptorID);
					descriptorSetUpdates.push_back(buffers);
				}

				//std::vector<VkDrawIndexedIndirectCommand> batchDraws;
				for (const auto& meshInstance : batchID.renderMeshInstances) //MESH INSTANCES GROUP
				{
					//  SAME MATERIAL + DIFFERENT MESHES
					// --- 
					
					//Move this to function, build a unique buffer with all instanced objects to draw in a single call
					auto vbuffer = static_cast<IUniformBuffer*>(meshInstance.meshObject->meshBuffers.vertexBuffer.get());
					auto vmembuffer = dynamic_cast<MemBuffer*>(vbuffer);
					auto ibuffer = static_cast<IUniformBuffer*>(meshInstance.meshObject->meshBuffers.indexBuffer.get());
					auto imembuffer = dynamic_cast<MemBuffer*>(ibuffer);

					VkDeviceSize offset = 0 ; //batch render
					vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vmembuffer->bufferObj, &offset);
					//vkCmdBindVertexBuffers(commandBuffer, 1, 1, nullptr, &offset);
					vkCmdBindIndexBuffer(commandBuffer, imembuffer->bufferObj, 0, VK_INDEX_TYPE_UINT32);

					std::vector<glm::mat4> instanceTransforms;
					for (const auto& entityInst : meshInstance.instancedMeshEntities) { //ENTITY SPECIFIC 
						//Mesh Instance & Data Updates :
						//SAME MATERIAL + SAME MESHES

						auto transform = entityInst->GetComponent<Transform>();
						transform->bufferModelMatrix->updateBufferData(&transform->getModelMatrix()[0]);
						auto modelViewDescriptor = gpipeline->getDescriptorSetWrite(transform->bufferModelMatrix.get(), PerMaterialInstance, 2); //<< SetID & DescriptorID need to be dynamic!
						descriptorSetUpdates.push_back(modelViewDescriptor);
						//instanceTransforms.push_back(transform->getModelMatrix());

					}//ENTITY SPECIFIC

					//Instanced Entity Transforms Descriptor:
					//bufferInstanceTransforms = std::make_shared<Vulkan::UniformBuffer>(instanceTransforms.data(), sizeof(glm::mat4), instanceTransforms.size(), BufferStorageDynamic);
					//auto instancedModelsDescriptor = gpipeline->getDescriptorSetWrite(&bufferInstanceTransforms, PerMaterialInstance, 2); //<< SetID & DescriptorID need to be dynamic!
					//descriptorSetUpdates.push_back(instancedModelsDescriptor);

					if (descriptorSetUpdates.size() != 0)
					{
						vkUpdateDescriptorSets(GraphicsHandler::get()->logicalDevice, descriptorSetUpdates.size(), descriptorSetUpdates.data(), 0, 0);
						gpipeline->bindDescriptorSets(commandBuffer);
						vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gpipeline->pipelineObj);

					}

					//VkDrawIndexedIndirectCommand drawInstance = {};
					//drawInstance.firstInstance = 0;
					//drawInstance.vertexOffset = 0;
					//drawInstance.indexCount = meshInstance.meshObject->meshData.indexData.size();
					//drawInstance.instanceCount = meshInstance.instancedMeshEntities.size();
					//
					//batchDraws.push_back(drawInstance);

					//gpipeline->bindDescriptorSets(commandBuffer);
					//vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gpipeline->pipelineObj);
					vkCmdDrawIndexed(commandBuffer, meshInstance.meshObject->meshData.indexData.size(), meshInstance.instancedMeshEntities.size(), 0, 0, 0);

					for (auto& var : descriptorSetUpdates)
					{
						if (var.pBufferInfo != NULL) {
							delete(var.pBufferInfo);
						}
						if (var.pImageInfo != NULL) {
							delete(var.pImageInfo);
						}
					}

				}//MESH INSTANCES

				//TODO: on draw indirect indexed, move pipeline and descriptor set binding to here!
				//UniformBuffer bufferBatchDraws = Vulkan::UniformBuffer(batchDraws.data(), sizeof(VkDrawIndexedIndirectCommand), batchDraws.size(), DrawIndirect);
				//bufferBatchDraws.updateBufferData(batchDraws.data());
				//vkCmdDrawIndexedIndirect(commandBuffer, bufferBatchDraws.bufferObj, 0, batchDraws.size(), 0);

			}//BATCH DRAW
			
		}

		graphicsInstance->swapchain->endRenderPassCommandBuffer(commandBuffer);

		FrameTime.Start();

	}

#pragma endregion

	void GraphicsContext::Draw()
	{
		//Wait for the previous frame to finish
		//Acquire an image from the swap chain
		//Record a command buffer which draws the scene onto that image
		//Submit the recorded command buffer
		//Present the swap chain image

		vkWaitForFences(graphicsInstance->logicalDevice, 1, &graphicsInstance->swapchain->getCurrentFrameFence(), VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(
			graphicsInstance->logicalDevice, 
			graphicsInstance->swapchain->swapChainObj, 
			UINT64_MAX, 
			graphicsInstance->swapchain->getCurrentFrameAvailableSemaphore(),
			VK_NULL_HANDLE, 
			&imageIndex //refers to vkGetSwapchainImagesKHR of swapchain ImageViews
		);

		if (result != VK_SUCCESS) {
			COMPHILOG_CORE_ERROR("failed to acquireNextImage!");
			//throw std::runtime_error("failed to acquireNextImage!");
			if (result == VK_SUBOPTIMAL_KHR) {
				COMPHILOG_CORE_ERROR("failed to acquire swap chain image!");
				throw std::runtime_error("failed to acquire swap chain image!");
			}
		}
		else {
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				graphicsInstance->swapchain->recreateSwapChain();
				return;
			}
		}

		// Only reset the fence if we are submitting work
		vkResetFences(graphicsInstance->logicalDevice, 1, &graphicsInstance->swapchain->getCurrentFrameFence());

		//vkResetCommandPool(graphicsInstance->logicalDevice, commandPool->graphicsCommandPool,0); 
		//if you are making multiple command buffers from one pool, resetting the pool will be quicker.
		//It can be implicitly reset when calling vkBeginCommandBuffer on the render loop
		vkResetCommandBuffer(graphicsInstance->swapchain->getCurrentFrameGraphicsCommandBuffer(), 0);

		//Scene Update
		updateSceneLoop();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { graphicsInstance->swapchain->getCurrentFrameAvailableSemaphore()};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &graphicsInstance->swapchain->getCurrentFrameGraphicsCommandBuffer();

		VkSemaphore signalSemaphores[] = { graphicsInstance->swapchain->getCurrentFrameFinishedSemaphore()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsInstance->graphicsQueue, 1, &submitInfo, graphicsInstance->swapchain->getCurrentFrameFence()) != VK_SUCCESS) {
			COMPHILOG_CORE_WARN("failed to submit draw command buffer!");
			//throw std::runtime_error("failed to submit draw command buffer!");
			//return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		
		VkSwapchainKHR swapChains[] = { graphicsInstance->swapchain->swapChainObj };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional error handling

		result = vkQueuePresentKHR(graphicsInstance->presentQueue, &presentInfo);

		/* TODO : Fix ImageView Presentation ready-ness on Framebuffer Resize
		* VK_validation layer: Validation Error: [ VUID-VkPresentInfoKHR-pImageIndices-01296 ] 
		Object 0: handle = 0x1dec32603e0, type = VK_OBJECT_TYPE_QUEUE; | MessageID = 0xc7aabc16 | vkQueuePresentKHR(): 
		pSwapchains[0] images passed to present must be in layout VK_IMAGE_LAYOUT_PRESENT_SRC_KHR or VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR but is in VK_IMAGE_LAYOUT_UNDEFINED. 
		The Vulkan spec states: Each element of pImageIndices must be the index of a presentable image acquired from the swapchain 
		specified by the corresponding element of the pSwapchains array, 
		and the presented image subresource must be in the VK_IMAGE_LAYOUT_PRESENT_SRC_KHR layout 
		at the time the operation is executed on a VkDevice (https://github.com/KhronosGroup/Vulkan-Docs/search?q=)VUID-VkPresentInfoKHR-pImageIndices-01296)
		*/

		//https://stackoverflow.com/questions/52915897/vulkan-at-vkqueuepresentkhr-the-validation-layer-throws-an-error-message-rega

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
			_framebufferResized = false;
			graphicsInstance->swapchain->recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to present swap chain image!");
			throw std::runtime_error("failed to present swap chain image!"); 
		}

		graphicsInstance->swapchain->incrementSwapChainFrame();

		//throw std::runtime_error("The End of the World");
	}

	void GraphicsContext::CleanUp()
	{
		vkDeviceWaitIdle(graphicsInstance->logicalDevice);

		//TODO : create Cleanup Stack of all Instanced Engine Objects (send vk objRefs to static queue on creation?)
		GraphicsHandler::get()->DeleteStatic();
		graphicsInstance->cleanUp();
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		ResizeFramebuffer(x, y);
	}

	void GraphicsContext::ResizeFramebuffer(uint x, uint y)
	{
		_framebufferResized = true;
	}
}