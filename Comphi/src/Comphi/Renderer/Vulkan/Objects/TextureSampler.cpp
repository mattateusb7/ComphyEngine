#include "cphipch.h"
#include "TextureSampler.h"

namespace Comphi::Vulkan {

	TextureSampler::TextureSampler(const ImageView& textureImageView, const std::shared_ptr<GraphicsHandler>& graphicsHandler)
	{
		this->graphicsHandler = graphicsHandler;
		this->textureImageView = std::make_shared<ImageView>(textureImageView);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;

		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(*graphicsHandler->physicalDevice.get(), &properties);

		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

		samplerInfo.unnormalizedCoordinates = VK_FALSE;//[0..1]UVW

		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(*graphicsHandler->logicalDevice.get(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
		COMPHILOG_CORE_INFO("Created TextureSampler successfully!");
	}

	TextureSampler::~TextureSampler() {
		cleanUp();
	}

	void TextureSampler::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy textureSampler");
		vkDestroySampler(*graphicsHandler->logicalDevice.get(), textureSampler, nullptr);
	}

}