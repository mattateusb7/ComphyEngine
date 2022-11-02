#include "cphipch.h"
#include "DescriptorPool.h"
#include <Comphi/Renderer/IObjects/IUniformBuffer.h>

namespace Comphi::Vulkan {

#pragma region DescriptorPool

	DescriptorPool::DescriptorPool(int MAX_FRAMES_IN_FLIGHT)
	{
		createDescriptorSetLayout();
		createDescriptorPool(MAX_FRAMES_IN_FLIGHT);
	}

	void DescriptorPool::createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional : relevant for image sampling

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(*GraphicsHandler::get()->logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create descriptor set layout!");
			throw std::runtime_error("failed to create descriptor set layout!");
		}

	}

	void DescriptorPool::createDescriptorPool(int MAX_FRAMES_IN_FLIGHT)
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		if (vkCreateDescriptorPool(*GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &descriptorPoolObj) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create descriptor pool!");
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}                                                                                                                                                                                                                      

	void DescriptorPool::updateDescriptorSet(MeshObject& obj, int MAX_FRAMES_IN_FLIGHT)
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPoolObj;
		allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(*GraphicsHandler::get()->logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to allocate descriptor sets!");
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) { //move To MeshObject

			//Next DrawCall Uniform DESCRIPTORS

			//OBJECT VERTEX
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = obj.ubos[i].get()->bufferObj;
			
			//TODO:
			//Try to understand how scene objects & uniformBuffers fit here...
			//maybe single uniform buffer for multiple object instantiaion ?
			
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			//OBJECT TEXTURES
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = obj.texture.get()->imageViewObj;
			imageInfo.sampler = obj.texture.get()->textureSamplerObj;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(*GraphicsHandler::get()->logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}


#pragma endregion

}