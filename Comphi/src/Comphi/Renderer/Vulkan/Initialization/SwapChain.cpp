#include "cphipch.h"
#include "SwapChain.h"

namespace Comphi::Vulkan {

	SwapChain::SwapChain()
	{
		GraphicsHandler::get()->setSwapchainHandler(MAX_FRAMES_IN_FLIGHT);
		createSwapChain();
		createRenderPass();
		createFramebuffers();
	}

	void SwapChain::createSwapChain() {

		COMPHILOG_CORE_TRACE("Creating Swapchain...");
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*GraphicsHandler::get()->physicalDevice.get(), *GraphicsHandler::get()->surface.get());

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		//Prevent waiting of img aloc from driver
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = *GraphicsHandler::get()->surface.get();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1; //1 unless stereoscopic 3D application.
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //post-processing : may use a value like VK_IMAGE_USAGE_TRANSFER_DST_BIT

		uint32_t queueFamilyIndices[] = { *GraphicsHandler::get()->graphicsQueueFamily.index, *GraphicsHandler::get()->transferQueueFamily.index }; //indices.presentFamily.value() == graphicsFamily

		//uint32_t uniqueQueueCount = 0;
		//if (indices.graphicsFamily != indices.transferFamily) uniqueQueueCount += 1;
		//if (indices.graphicsFamily != indices.presentFamily) uniqueQueueCount += 1;
		//if (indices.presentFamily != indices.transferFamily) uniqueQueueCount += 1;

		if (*GraphicsHandler::get()->graphicsQueueFamily.index != *GraphicsHandler::get()->transferQueueFamily.index) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE; // ignore color of obscured pixels (another window)

		createInfo.oldSwapchain = VK_NULL_HANDLE;
		//swap chain becomes invalid if window was resized

		if (vkCreateSwapchainKHR(*GraphicsHandler::get()->logicalDevice.get(), &createInfo, nullptr, &swapChainObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create swap chain!");
			throw std::runtime_error("failed to create swap chain");
			return;
		}

		COMPHILOG_CORE_INFO("SwapChain created Successfully!");

		vkGetSwapchainImagesKHR(*GraphicsHandler::get()->logicalDevice.get(), swapChainObj, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(*GraphicsHandler::get()->logicalDevice.get(), swapChainObj, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		createImageViews();
	}

	void SwapChain::createImageViews() {
		COMPHILOG_CORE_TRACE("Creating ImageViews...");
		swapChainImageViews.resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			swapChainImageViews[i].initSwapchainImageView(swapChainImages[i], swapChainImageFormat);
			swapChainImageViews[i].imageExtent = swapChainExtent;
		}
		swapChainDepthView = ImageView();
		swapChainDepthView.initDepthImageView(swapChainImageViews[0]);
	}

	void SwapChain::recreateSwapChain() {

		//It is possible to create a new swap chain while drawing commands on an image from the old swap chain are still in-flight. 
		//You need to pass the previous swap chain to the oldSwapChain field in the VkSwapchainCreateInfoKHR struct and destroy the old swap chain as soon as you've finished using it.
		int width = 0, height = 0;
		glfwGetFramebufferSize(GraphicsHandler::get()->windowHandle, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(GraphicsHandler::get()->windowHandle, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(*GraphicsHandler::get()->logicalDevice);

		cleanUp();
		createSwapChain();
		createFramebuffers();
	}

	void SwapChain::cleanUp() {
		short fbid = 0;
		for (auto framebuffer : swapChainFramebuffers) {
			COMPHILOG_CORE_INFO("vkDestroy Destroy framebuffer {0}", fbid++);
			vkDestroyFramebuffer(*GraphicsHandler::get()->logicalDevice, framebuffer, nullptr);
		}

		int n_img = 0;
		for (auto imageView : swapChainImageViews) {
			imageView.cleanUp();
		}

		COMPHILOG_CORE_INFO("vkDestroy Destroy Swapchain:");
		vkDestroySwapchainKHR(*GraphicsHandler::get()->logicalDevice.get(), swapChainObj, nullptr);
	}


	SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}
		//https://vkguide.dev/docs/chapter-1/vulkan_init_flow/#swapchain
		//Strong VSync (locked to Screen refresh rate)
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		//function-like macros require the token after the macro name to be a left parenthesis in order to expand. 
		//Wrapping the name in parentheses is really just a hack to make the next token a right parenthesis
		//without changing the meaning once you put macros aside.
		if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(GraphicsHandler::get()->windowHandle, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

#pragma region Framebuffer

	void SwapChain::createFramebuffers() {
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			std::array<VkImageView, 2> attachments = {
				swapChainImageViews[i].imageViewObj,
				swapChainDepthView.imageViewObj
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPassObj;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(*GraphicsHandler::get()->logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				COMPHILOG_CORE_FATAL("failed to create framebuffer!");
				throw std::runtime_error("failed to create framebuffer!");
				return;
			}
			COMPHILOG_CORE_INFO("created framebuffer of imageView {0}!",i);
		}
	}

	void SwapChain::createRenderPass()
	{
		//VkImage Render Attatchments
		//ColorAttachment
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		//DepthAttachment
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = swapChainDepthView.imageFormat;
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		//SubPasses
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		//RenderPass
		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		//RenderPass Dependency
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(*GraphicsHandler::get()->logicalDevice, &renderPassInfo, nullptr, &renderPassObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create render pass!");
			throw std::runtime_error("failed to create render pass!");
			return;
		}

		COMPHILOG_CORE_INFO("created RenderPass Successfully!");
		GraphicsHandler::get()->setRenderPass(renderPassObj);

	}

	void SwapChain::incrementSwapChainFrame()
	{
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void SwapChain::recordCommandBuffer(VkCommandBuffer commandBuffer, MeshObject& meshObj, uint32_t imageIndex) {

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
		renderPassInfo.renderPass = renderPassObj;
		renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		std::array<VkClearValue, 2> clearValues{}; //same order as attachments
		clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();


		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{//begin render pass

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, meshObj.material->graphicsPipeline.pipelineObj);

			//Bind VertexBuffers @0
			VkBuffer vertexBuffers[] = { meshObj.vertices->bufferObj };
			VkDeviceSize offsets[] = { 0 }; //batch render
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			//Bind IndexBuffers @1
			vkCmdBindIndexBuffer(commandBuffer, meshObj.indices->bufferObj, 0, VK_INDEX_TYPE_UINT32);

			//dynamic VIEWPORT/SCISSOR SETUP
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(swapChainExtent.width);
			viewport.height = static_cast<float>(swapChainExtent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChainExtent;
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			//BindIndex UniformBuffers 
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, meshObj.material->graphicsPipeline.pipelineLayout, 0, 1, &meshObj.material->graphicsPipeline.descriptorSets[currentFrame], 0, nullptr);

			//DRAW COMMAND
			vkCmdDrawIndexed(commandBuffer, meshObj.indices->indexCount, 1, 0, 0, 0);
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

	SwapChain::~SwapChain()
	{
		cleanUp();
		COMPHILOG_CORE_INFO("vkDestroy Destroy RenderPass");
		vkDestroyRenderPass(*GraphicsHandler::get()->logicalDevice, renderPassObj, nullptr);
	}

#pragma endregion
}
