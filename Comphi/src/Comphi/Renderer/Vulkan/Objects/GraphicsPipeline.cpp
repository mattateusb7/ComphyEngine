#include "cphipch.h"
#include "GraphicsPipeline.h"

namespace Comphi::Vulkan {

	void GraphicsPipeline::initialize(GraphicsPipelineConfiguration config)
	{
		//createDescriptorPool();
		//createDescriptorSetLayout();

		//TODO: Separate initialization to list of different pipeline creation stages (functions);
		//https://vkguide.dev/docs/chapter-2/pipeline_walkthrough/

		//TODO: this seems like a good place to start working on dynamic Descriptor Pools
		
		//---------- VertexBufferDescriptions
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = std::vector<VkVertexInputBindingDescription>(config.vertexInputDescription.attributeDescriptionsBindings.size());
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = std::vector<VkVertexInputAttributeDescription>(config.vertexInputDescription.attributeDescriptionsBindings.size());

		for (size_t i = 0; i < config.vertexInputDescription.bindingDescriptions.size(); i++) {
			BindingDescription& binding = config.vertexInputDescription.bindingDescriptions[i];
			bindingDescriptions[i].binding = binding.ID;
			bindingDescriptions[i].stride = binding.stride;
			bindingDescriptions[i].inputRate = (VkVertexInputRate)binding.inputRate;
			
			for (size_t i = 0; i < config.vertexInputDescription.attributeDescriptionsBindings.size(); i++)
			{
				AttributeBinding& attribute = config.vertexInputDescription.attributeDescriptionsBindings[i];
				attributeDescriptions[i].binding = attribute.ID;
				attributeDescriptions[i].location = attribute.location;
				attributeDescriptions[i].format = attribute.format;
				attributeDescriptions[i].offset = attribute.offset;
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint>(bindingDescriptions.size());
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		//----------


		//---------- Primitives
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = (VkPrimitiveTopology)config.topologyType;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		//----------


		//---------- Framebuffer/Viewport(Scissor)
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

			//Immutable ViewPort (requires reconstruction of pipeline)
			//VkPipelineViewportStateCreateInfo viewportState{};
			//viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			//viewportState.viewportCount = 1;
			//viewportState.pViewports = &viewport; &graphicsPipelineSetupData.viewport;
			//viewportState.scissorCount = 1;
			//viewportState.pScissors = &scissor; &graphicsPipelineSetupData.scissor;

		//----------

		//Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = (VkPolygonMode)config.rasterizerSettings.polygonRenderMode; 
		rasterizer.lineWidth = config.rasterizerSettings.lineWidth;
		rasterizer.cullMode = (VkCullModeFlags)config.rasterizerSettings.cullMode;
		rasterizer.frontFace = (VkFrontFace)config.rasterizerSettings.frontFace;
		rasterizer.depthBiasEnable = VK_FALSE; //For Shadow mapping
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		//Multisampling 
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //to enable MSAA, you would need to set rasterizationSamples to more than 1, and enable sampleShading
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		//----------
		


		//ColorBlending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		switch (config.rasterizerSettings.blendingMode)
		{
		case ColorBlendingModes::AlphaBlend:
			//finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
			//finalColor.a = newAlpha.a;
			colorBlendAttachment.blendEnable = VK_TRUE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			break;
		case ColorBlendingModes::RGBMixing:
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
			break;
		default:
			break;
		}
		//bitwise combination
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE; //TOGGLE
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional



		//DepthStencilBuffer
		VkPipelineDepthStencilStateCreateInfo depthStencil{};//TODO: Add Stencil Shader Support "Cutouts"
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {}; // Optional	stencil : make sure that the format of the depth/stencil image contains a stencil component.
		depthStencil.back = {}; // Optional		stencil : make sure that the format of the depth/stencil image contains a stencil component.

		{
			/***
			//TODO: Add DescriptonSetLayoutProperties Struct in the future to allow diferent layouts
			/This constructor defines DescriptorPool Compatibility with Descriptor Sets Layouts
			/All Descriptor Sets in this Graphics Pipeline Should Be Compatible with this Descriptor Pool
			***/
			//createDescriptorPool()
			int MAX_FRAMES_IN_FLIGHT = *GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT;
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

			vkCheckError(vkCreateDescriptorPool(GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &descriptorPoolObj)) {
				COMPHILOG_CORE_FATAL("failed to create descriptor pool!");
				throw std::runtime_error("failed to create descriptor pool!");
			}

			//createDescriptorSetLayout()

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

			vkCheckError(vkCreateDescriptorSetLayout(GraphicsHandler::get()->logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout)) {
				COMPHILOG_CORE_FATAL("failed to create descriptor set layout!");
				throw std::runtime_error("failed to create descriptor set layout!");
			}

			//Pipelineslayout
			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
			pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
			pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

			vkCheckError(vkCreatePipelineLayout(GraphicsHandler::get()->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout)) {
				COMPHILOG_CORE_FATAL("failed to create pipeline layout!");
				throw std::runtime_error("failed to create pipeline layout!");
			}

			COMPHILOG_CORE_INFO("created pipelineLayout successfully!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = config.shaderPrograms.data();
		//pipelineInfo.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = *GraphicsHandler::get()->renderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		vkCheckError(vkCreateGraphicsPipelines(GraphicsHandler::get()->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipelineObj)) {
			COMPHILOG_CORE_FATAL("failed to create graphics pipeline!");
			throw std::runtime_error("failed to create graphics layout!");
		}
		COMPHILOG_CORE_INFO("created graphics pipeline successfully!");
	}

	void GraphicsPipeline::sendDescriptorSet()
	{

		//INPUT RESOURCES 
		std::vector<ITexture*>& textures
		std::vector<UniformBuffer>& MVP_ubos
		//---

		int MAX_FRAMES_IN_FLIGHT = *GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT;
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
		
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPoolObj;
		allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		vkCheckError(vkAllocateDescriptorSets(GraphicsHandler::get()->logicalDevice, &allocInfo, descriptorSets.data())) {
			COMPHILOG_CORE_FATAL("failed to allocate descriptor sets!");
			return;
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) { //move To MeshObject

			//Next DrawCall Uniform DESCRIPTORS

			//OBJECT VERTEX
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = MVP_ubos[i].bufferObj;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			//OBJECT TEXTURES
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = static_cast<ImageView*>(textures[0])->imageView;
			imageInfo.sampler = static_cast<ImageView*>(textures[0])->textureSampler;

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

			vkUpdateDescriptorSets(GraphicsHandler::get()->logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
		COMPHILOG_CORE_INFO("vkUpdateDescriptorSets success!");
	}

	void GraphicsPipeline::cleanUp()
	{
		COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorPool");
		vkDestroyDescriptorPool(Vulkan::GraphicsHandler::get()->logicalDevice, descriptorPoolObj, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorSetLayout");
		vkDestroyDescriptorSetLayout(Vulkan::GraphicsHandler::get()->logicalDevice, descriptorSetLayout, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy PipelineLayout");
		vkDestroyPipelineLayout(GraphicsHandler::get()->logicalDevice, pipelineLayout, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy graphicsPipeline");
		vkDestroyPipeline(GraphicsHandler::get()->logicalDevice, pipelineObj, nullptr);

	}

}