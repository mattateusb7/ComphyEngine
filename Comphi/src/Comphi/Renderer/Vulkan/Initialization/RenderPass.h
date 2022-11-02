#pragma once

#include "../GraphicsHandler.h"
#include "DescriptorPool.h"

namespace Comphi::Vulkan {
	class RenderPass
	{
	public:
		RenderPass(VkFormat swapChainImageFormat, VkFormat swapChainDepthViewFormat, int MAX_FRAMES_IN_FLIGHT);
		VkRenderPass renderPassObj;
		std::unique_ptr<DescriptorPool> descriptorPool;
		~RenderPass();
	};
}

