#include "cphipch.h"
#include "GraphicsPipeline.h"
#include "Comphi/Renderer/Vulkan/Graphics/ShaderProgram.h"
	
namespace Comphi::Vulkan {

	void GraphicsPipeline::initialize() 
	{
		//TODO: Move all this code to separate Functions
		
		//---------- VertexBufferDescriptions
		size_t vertexBindingDescriptionCount = configuration.vertexInputLayoutConfiguration.vertexBufferBindingDescriptors.size();
		size_t vertexAttributeBindingsCount = configuration.vertexInputLayoutConfiguration.vertexAttributeFormatDescriptors.size();

		std::vector<VkVertexInputBindingDescription> vertexBufferBindingDescriptors = std::vector<VkVertexInputBindingDescription>(vertexBindingDescriptionCount);
		std::vector<VkVertexInputAttributeDescription> attributeDescriptors = std::vector<VkVertexInputAttributeDescription>(vertexAttributeBindingsCount);

		for (size_t i = 0; i < vertexBindingDescriptionCount; i++) {
			VertexBufferBindingDescription& binding = configuration.vertexInputLayoutConfiguration.vertexBufferBindingDescriptors[i];
			vertexBufferBindingDescriptors[i].binding = binding.bufferBindingID;
			vertexBufferBindingDescriptors[i].stride = binding.vertexStride;
			vertexBufferBindingDescriptors[i].inputRate = (VkVertexInputRate)binding.inputRate;
		}

		for (size_t i = 0; i < vertexAttributeBindingsCount; i++)
		{
			VertexAttributeBindingDescription& attribute = configuration.vertexInputLayoutConfiguration.vertexAttributeFormatDescriptors[i];
			attributeDescriptors[i].binding = attribute.bufferBindingID;
			attributeDescriptors[i].location = attribute.shaderLocationID;
			attributeDescriptors[i].format = (VkFormat)attribute.format;
			attributeDescriptors[i].offset = attribute.offset;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount;
		vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributeBindingsCount;
		vertexInputInfo.pVertexBindingDescriptions = vertexBufferBindingDescriptors.data();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptors.data();
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
		inputAssembly.topology = (VkPrimitiveTopology)configuration.assemblySettings.topologyType;
		inputAssembly.primitiveRestartEnable = VK_FALSE;


		//Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = (VkPolygonMode)configuration.rasterizerSettings.polygonRenderMode; 
		rasterizer.lineWidth = configuration.rasterizerSettings.lineWidth;
		rasterizer.cullMode = (VkCullModeFlags)configuration.rasterizerSettings.cullMode;
		rasterizer.frontFace = (VkFrontFace)configuration.rasterizerSettings.frontFace;
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
		switch (configuration.rasterizerSettings.blendingMode)
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
		//and bind Materials(groups of shaders) to them through GraphicsPipeline*
		
		//How many Descriptor Sets per PipelineLayoutSet ?
		//TODO: Test if we can have N DescriptorSetLayouts and One DescriptorPool per descriptorSet Layout

		//Dynamic DescriptorSetLayout & Pool Creation !
		size_t MAX_FRAMES_IN_FLIGHT = static_cast<uint>(*GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT); //TODO: Validate this with some tests
		
		size_t layoutSetsCount = configuration.pipelineLayoutConfiguration.layoutSets.size();
		pipelineSetLayouts = std::vector<LayoutSet>(layoutSetsCount);
		auto descriptorSetLayouts = std::vector<VkDescriptorSetLayout>(layoutSetsCount);

		std::vector<VkDescriptorPoolSize> poolSizes;
		uint poolSizesMaxSets = 0;
		for (size_t i = 0; i < layoutSetsCount; i++)
		{
			pipelineSetLayouts[i].descriptorSetBindingsCount = configuration.pipelineLayoutConfiguration.layoutSets[i].shaderResourceDescriptorSetBindings.size();
			pipelineSetLayouts[i].descriptorSetBindings = std::vector<VkDescriptorSetLayoutBinding>(pipelineSetLayouts[i].descriptorSetBindingsCount);
			auto& descriptorSetBindings = pipelineSetLayouts[i].descriptorSetBindings;

			for (size_t n = 0; n < pipelineSetLayouts[i].descriptorSetBindingsCount; n++)
			{
				//Descriptor Sets Layout data
				DescriptorSetBinding& descriptorSet = configuration.pipelineLayoutConfiguration.layoutSets[i].shaderResourceDescriptorSetBindings[n];
				descriptorSetBindings[n].binding = n;
				descriptorSetBindings[n].descriptorType = (VkDescriptorType)descriptorSet.resourceType;
				descriptorSetBindings[n].descriptorCount = descriptorSet.resourceCount;
				descriptorSetBindings[n].stageFlags = (VkShaderStageFlags)descriptorSet.shaderStage;
				descriptorSetBindings[n].pImmutableSamplers = nullptr; // Optional : relevant for image sampling

				//Descriptor Pool Allocation data
				VkDescriptorPoolSize descriptorPoolSize;
				descriptorPoolSize.type = (VkDescriptorType)descriptorSet.resourceType;
				descriptorPoolSize.descriptorCount = descriptorSet.resourceCount * MAX_FRAMES_IN_FLIGHT;
				poolSizes.push_back(descriptorPoolSize);
				poolSizesMaxSets += descriptorPoolSize.descriptorCount;

				COMPHILOG_CORE_INFO("created descriptorSet {0} !", n);

			}

			//Create Layout Set
			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = pipelineSetLayouts[i].descriptorSetBindingsCount;
			layoutInfo.pBindings = descriptorSetBindings.data();

			vkCheckError(vkCreateDescriptorSetLayout(GraphicsHandler::get()->logicalDevice, &layoutInfo, nullptr, &pipelineSetLayouts[i].descriptorSetLayout)) {
				COMPHILOG_CORE_FATAL("failed to create descriptor set layout!");
				throw std::runtime_error("failed to create descriptor set layout!");
			}

			COMPHILOG_CORE_INFO("created LayoutSet {0} !", i);

			descriptorSetLayouts[i] = pipelineSetLayouts[i].descriptorSetLayout;
		}

		//Create Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = layoutSetsCount; //Descriptor set ID count
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data(); //Descriptor set IDs ptr (layout(set = #))
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		vkCheckError(vkCreatePipelineLayout(GraphicsHandler::get()->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout)) {
			COMPHILOG_CORE_FATAL("failed to create pipeline layout!");
			throw std::runtime_error("failed to create pipeline layout!");
		}

		COMPHILOG_CORE_INFO("created pipelineLayout successfully!");

		//Allocate DescriptorsPool 
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = poolSizesMaxSets;

		vkCheckError(vkCreateDescriptorPool(GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &pipelineDescriptorPool)) {
			COMPHILOG_CORE_FATAL("failed to create descriptor pool!");
			throw std::runtime_error("failed to create descriptor pool!");
		};

		COMPHILOG_CORE_INFO("allocated Material DescriptorPool successfully!");

		//Allocate DescriptorSets
		for (size_t i = 0; i < layoutSetsCount; i++)
		{
			if (pipelineSetLayouts[i].descriptorSetBindingsCount == 0) continue; //skip dummies
			
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.descriptorPool = pipelineDescriptorPool;
			allocInfo.pSetLayouts = &pipelineSetLayouts[i].descriptorSetLayout;
			vkCheckError(vkAllocateDescriptorSets(GraphicsHandler::get()->logicalDevice, &allocInfo, &pipelineSetLayouts[i].descriptorSet)) {
				COMPHILOG_CORE_FATAL("failed to allocate descriptor sets!");
				return;
			}
			COMPHILOG_CORE_INFO("allocated DescriptorSets of Layout {0} successfully!", i);

		}

		size_t stageCount = configuration.pipelineLayoutConfiguration.shaderPrograms.size();
		std::vector<VkPipelineShaderStageCreateInfo> shaderStagesInfo = std::vector<VkPipelineShaderStageCreateInfo>(stageCount);
		for (size_t i = 0; i < stageCount; i++)
		{
			ShaderProgram* _shaderProgram = static_cast<ShaderProgram*>(configuration.pipelineLayoutConfiguration.shaderPrograms[i]);

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

	VkWriteDescriptorSet GraphicsPipeline::getDescriptorSetWrite(void* dataObjectsArray, LayoutSetUpdateFrequency setID, uint descriptorID)
	{
		//https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkWriteDescriptorSet.html

		DescriptorSetBinding& descriptorSet = getDescriptorSet(setID, descriptorID);

		VkWriteDescriptorSet descriptorWrite;
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = pipelineSetLayouts[setID].descriptorSet;
		descriptorWrite.dstBinding = descriptorID;
		descriptorWrite.dstArrayElement = 0;
		//dstArrayElement is the starting element in that array.
		//If the descriptor binding identified by dstSet and dstBinding has a 
		//descriptor type of VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK then dstArrayElement 
		//specifies the starting byte offset within the binding.

		switch (descriptorSet.resourceType)
		{
		case DescriptorSetResourceType::UniformBufferData:
		{
			MemBuffer* buffer = static_cast<MemBuffer*>(dataObjectsArray);

			std::vector<VkDescriptorBufferInfo> buffersInfo;
			buffersInfo.resize(descriptorSet.resourceCount);
			for (size_t i = 0; i < descriptorSet.resourceCount; i++)
			{
				buffersInfo[i].buffer = buffer[i].bufferObj;
				buffersInfo[i].range = buffer[i].bufferSize;
				buffersInfo[i].offset = 0;
			}
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = descriptorSet.resourceCount;
			descriptorWrite.pBufferInfo = buffersInfo.data();
			break;
		}

		case DescriptorSetResourceType::ImageBufferSampler:
		{	

			auto imageArr = reinterpret_cast<ImageView*>(dataObjectsArray);

			std::vector<VkDescriptorImageInfo> imageSamplers;
			imageSamplers.resize(descriptorSet.resourceCount);
			for (size_t i = 0; i < descriptorSet.resourceCount; i++)
			{
				imageSamplers[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageSamplers[i].imageView = imageArr[i].imageView;
				imageSamplers[i].sampler = imageArr[i].textureSampler;
			}

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = descriptorSet.resourceCount;
			descriptorWrite.pImageInfo = imageSamplers.data();
			break;
		}

		default:
			break;
		}

		return descriptorWrite;

	}

	void GraphicsPipeline::cleanUp()
	{
		for (size_t i = 0; i < pipelineSetLayouts.size(); i++)
		{
			COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorPool");
			vkDestroyDescriptorPool(Vulkan::GraphicsHandler::get()->logicalDevice, pipelineDescriptorPool, nullptr);
			pipelineSetLayouts[i].descriptorSetBindings.clear(); //pipeline clears descriptor sets :3

			COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorSetLayout");
			vkDestroyDescriptorSetLayout(Vulkan::GraphicsHandler::get()->logicalDevice, pipelineSetLayouts[i].descriptorSetLayout, nullptr);
		}

		COMPHILOG_CORE_INFO("vkDestroy Destroy PipelineLayout");
		vkDestroyPipelineLayout(GraphicsHandler::get()->logicalDevice, pipelineLayout, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy graphicsPipeline");
		vkDestroyPipeline(GraphicsHandler::get()->logicalDevice, pipelineObj, nullptr);

	}

	

}