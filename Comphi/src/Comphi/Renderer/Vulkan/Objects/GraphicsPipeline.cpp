#include "cphipch.h"
#include "GraphicsPipeline.h"

namespace Comphi::Vulkan {

	void GraphicsPipeline::initialize(GraphicsPipelineConfiguration config)
	{
		//TODO: Move all this code to separate Functions
		
		//---------- VertexBufferDescriptions
		uint vertexBindingDescriptionCount = config.vertexInputDescription.bindingDescriptions.size();
		uint vertexAttributeBindingsCount = config.vertexInputDescription.attributeDescriptionsBindings.size();

		std::vector<VkVertexInputBindingDescription> bindingDescriptions = std::vector<VkVertexInputBindingDescription>(vertexBindingDescriptionCount);
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = std::vector<VkVertexInputAttributeDescription>(vertexAttributeBindingsCount);

		for (size_t i = 0; i < vertexBindingDescriptionCount; i++) {
			VertexBindingDescription& binding = config.vertexInputDescription.bindingDescriptions[i];
			bindingDescriptions[i].binding = binding.ID;
			bindingDescriptions[i].stride = binding.stride;
			bindingDescriptions[i].inputRate = (VkVertexInputRate)binding.inputRate;
			
			for (size_t i = 0; i < vertexAttributeBindingsCount; i++)
			{
				VertexAttributeBinding& attribute = config.vertexInputDescription.attributeDescriptionsBindings[i];
				attributeDescriptions[i].binding = attribute.ID;
				attributeDescriptions[i].location = attribute.location;
				attributeDescriptions[i].format = (VkFormat)attribute.format;
				attributeDescriptions[i].offset = attribute.offset;
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount;
		vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributeBindingsCount;
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
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

		//Primitives
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = (VkPrimitiveTopology)config.assemblySettings.topologyType;
		inputAssembly.primitiveRestartEnable = VK_FALSE;


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
			//https://vkguide.dev/docs/chapter-2/pipeline_walkthrough/
			/***
			//TODO: Add DescriptonSetLayoutProperties Struct in the future to allow diferent layouts
			/This constructor defines DescriptorPool Compatibility with Descriptor Sets Layouts
			/All Descriptor Sets in this Graphics Pipeline Should Be Compatible with this Descriptor Pool
			***/
		
			//Pipelineslayout
			//the point of having multiple descriptor sets is to allow users to change one set of descriptors without changing another, 
			//and to allow pipelines to be partially layout compatible with one another.
			//https://stackoverflow.com/questions/56928041/what-is-the-purpose-of-multiple-setlayoutcounts-of-vulkan-vkpipelinelayoutcreate
			// Foreach Scene
			// Foreach Pipe{
			// 
			//	Custom Descriptor set Layout ex: 
			//	set 0 (GlobalResouces) : Constants
			//	set 1 (per-scene) : Camera Projection Matrix , Lights(unless we want masks of some kind)
			//	set 2 (per-Material static data) : Textures
			//	set 3 (per-Material dynamic (instance lvl) ) : parameters, ModelView matrix
			// 
			// }
			//https://computergraphics.stackexchange.com/questions/12481/a-lot-of-pipelines-on-vulkan-for-each-render-pass-what-could-go-wrong
			//Even if there are only a few pipelines per a frame, a program can still get into trouble by constantly switching between pipelines.
			//So, combine draw calls on a per pipe basis, meaning bind a pipe, do all the draws for that pipe, bind the next pipe, etc.
			
			//Im planning to use the GraphicPipelineLayouts as the "Phases" and the Materials 
			//will be variations of shader groups that fit those Layout descriptions of resources
			
			//TODO : The Client will be able to customize multiple PipelineLayouts through the ComphiAPI 
			//and bind Materias(groups of shaders) to them through GraphicsPipeline*
		
			//How many Descriptor Sets per PipelineLayoutSet ?
			//TODO: Test if i can have N DescriptorSetLayouts and One DescriptorPool per descriptorSet Layout

			//Dynamic DescriptorSetLayout & Pool Creation !
			uint pipelineLayoutSetCount = config.pipelineLayoutConfig.pipelineLayoutSets.size();
			descriptorSetLayouts = std::vector<VkDescriptorSetLayout>(pipelineLayoutSetCount);
			descriptorPools = std::vector<VkDescriptorPool>(pipelineLayoutSetCount);
			for (size_t i = 0; i < pipelineLayoutSetCount; i++)
			{

				uint DescriptorSetBindingsCount = config.pipelineLayoutConfig.pipelineLayoutSets[i].shaderResourceDescriptors.size();
				uint MAX_FRAMES_IN_FLIGHT = static_cast<uint>(*GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT); //TODO: Validate this with some tests

				std::vector<VkDescriptorPoolSize> descriptorPoolSizes = std::vector<VkDescriptorPoolSize>(DescriptorSetBindingsCount);
				std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings = std::vector<VkDescriptorSetLayoutBinding>(DescriptorSetBindingsCount);
				for (size_t n = 0; n < DescriptorSetBindingsCount; n++)
				{
					//Descriptor Set data
					ShaderResourceDescriptorSet& descriptorSet = config.pipelineLayoutConfig.pipelineLayoutSets[i].shaderResourceDescriptors[n];
					VkDescriptorSetLayoutBinding layoutBinding;
					descriptorSetLayoutBindings[n].binding = descriptorSet.bindingID;
					descriptorSetLayoutBindings[n].descriptorCount = descriptorSet.count;
					descriptorSetLayoutBindings[n].descriptorType = (VkDescriptorType)descriptorSet.type;
					descriptorSetLayoutBindings[n].stageFlags = (VkShaderStageFlags)descriptorSet.flags;
					descriptorSetLayoutBindings[n].pImmutableSamplers = nullptr; // Optional : relevant for image sampling

					//Descriptor Pool data
					descriptorPoolSizes[n].type = (VkDescriptorType)descriptorSet.type;
					descriptorPoolSizes[n].descriptorCount = MAX_FRAMES_IN_FLIGHT;
				}
				VkDescriptorSetLayoutCreateInfo layoutInfo{};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = DescriptorSetBindingsCount;
				layoutInfo.pBindings = descriptorSetLayoutBindings.data();

				vkCheckError(vkCreateDescriptorSetLayout(GraphicsHandler::get()->logicalDevice, &layoutInfo, nullptr, &descriptorSetLayouts[i])) {
					COMPHILOG_CORE_FATAL("failed to create descriptor set layout!");
					throw std::runtime_error("failed to create descriptor set layout!");
				}

				VkDescriptorPoolCreateInfo poolInfo{};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = DescriptorSetBindingsCount;
				poolInfo.pPoolSizes = descriptorPoolSizes.data();
				poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

				vkCheckError(vkCreateDescriptorPool(GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &descriptorPools[i])) {
					COMPHILOG_CORE_FATAL("failed to create descriptor pool!");
					throw std::runtime_error("failed to create descriptor pool!");
				}

			}

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = pipelineLayoutSetCount; //Descriptor set ID count
			pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data(); //Descriptor set IDs ptr (layout(set = #))
			pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
			pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

			vkCheckError(vkCreatePipelineLayout(GraphicsHandler::get()->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout)) {
				COMPHILOG_CORE_FATAL("failed to create pipeline layout!");
				throw std::runtime_error("failed to create pipeline layout!");
			}

			COMPHILOG_CORE_INFO("created pipelineLayout successfully!");
		}

		uint stageCount = config.pipelineLayoutConfig.shaderPrograms.size();
		std::vector<VkPipelineShaderStageCreateInfo> shaderStagesInfo = std::vector<VkPipelineShaderStageCreateInfo>(stageCount);
		for (size_t i = 0; i < stageCount; i++)
		{
			//Need to find a way to abstract Shader Modules after all ... DynamicCast It is !
			//Just have to guarantee that ComphiAPI Creates a full obj of type ShaderProgram
			ShaderProgram* _shaderProgram = dynamic_cast<ShaderProgram*>(config.pipelineLayoutConfig.shaderPrograms[i]);

			switch (_shaderProgram->GetType())
			{
			case (uint)Comphi::ShaderType::VertexShader: {
				//VERTEX
				shaderStagesInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStagesInfo[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
				shaderStagesInfo[i].module = _shaderProgram->shaderModule;
				shaderStagesInfo[i].pName = _shaderProgram->entryPointFunctionName.data();
				shaderStagesInfo[i].pSpecializationInfo = nullptr;
				break;
			}
			case (uint)Comphi::ShaderType::FragmentShader: {
				//FRAGMENT
				shaderStagesInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStagesInfo[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				shaderStagesInfo[i].module = _shaderProgram->shaderModule;
				shaderStagesInfo[i].pName = _shaderProgram->entryPointFunctionName.data();
				break;
			}
			default:
				break;
			}
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = stageCount;
		pipelineInfo.pStages = shaderStagesInfo.data();


		//pipelineInfo.pStages = shaderStages.data(); 
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