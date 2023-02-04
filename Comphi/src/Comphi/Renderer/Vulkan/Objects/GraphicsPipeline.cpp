#include "cphipch.h"
#include "GraphicsPipeline.h"
#include "Comphi/Renderer/Vulkan/Objects/ShaderProgram.h"
#include "../Objects/UniformBuffer.h"

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
		uint layoutSetsCount = config.pipelineLayoutConfiguration.layoutSets.size();
		uint MAX_FRAMES_IN_FLIGHT = static_cast<uint>(*GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT); //TODO: Validate this with some tests
		graphicsSetLayouts = std::vector<LayoutSet>(layoutSetsCount);
		for (size_t i = 0; i < layoutSetsCount; i++)
		{

			uint descriptorSetBindingsCount = config.pipelineLayoutConfiguration.layoutSets[i].shaderResourceDescriptorSets.size();

			std::vector<VkDescriptorPoolSize> descriptorPoolSizes = std::vector<VkDescriptorPoolSize>(descriptorSetBindingsCount);
			std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings = std::vector<VkDescriptorSetLayoutBinding>(descriptorSetBindingsCount);
			for (size_t n = 0; n < descriptorSetBindingsCount; n++)
			{
				//Descriptor Sets data
				ShaderResourceDescriptorSet& descriptorSet = config.pipelineLayoutConfiguration.layoutSets[i].shaderResourceDescriptorSets[n];
				descriptorSetLayoutBindings[n].binding = n;
				descriptorSetLayoutBindings[n].descriptorType = (VkDescriptorType)descriptorSet.type;
				descriptorSetLayoutBindings[n].descriptorCount = descriptorSet.count;
				descriptorSetLayoutBindings[n].stageFlags = (VkShaderStageFlags)descriptorSet.flags;
				descriptorSetLayoutBindings[n].pImmutableSamplers = nullptr; // Optional : relevant for image sampling

				//Descriptor Pools data
				descriptorPoolSizes[n].type = (VkDescriptorType)descriptorSet.type;
				descriptorPoolSizes[n].descriptorCount = descriptorSet.count * MAX_FRAMES_IN_FLIGHT; 

				VkDescriptorPoolCreateInfo poolInfo{};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = descriptorSetBindingsCount;
				poolInfo.pPoolSizes = descriptorPoolSizes.data();
				poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

				vkCheckError(vkCreateDescriptorPool(GraphicsHandler::get()->logicalDevice, &poolInfo, nullptr, &graphicsSetLayouts[i].descriptorPools[n])) {
					COMPHILOG_CORE_FATAL("failed to create descriptor pool!");
					throw std::runtime_error("failed to create descriptor pool!");
				};

			}

			//Create Layout Set
			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = descriptorSetBindingsCount;
			layoutInfo.pBindings = descriptorSetLayoutBindings.data();

			vkCheckError(vkCreateDescriptorSetLayout(GraphicsHandler::get()->logicalDevice, &layoutInfo, nullptr, &graphicsSetLayouts[i].descriptorSetLayout)) {
				COMPHILOG_CORE_FATAL("failed to create descriptor set layout!");
				throw std::runtime_error("failed to create descriptor set layout!");
			}

		}

		//Create Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = layoutSetsCount; //Descriptor set ID count
		pipelineLayoutInfo.pSetLayouts = getSetLayouts(graphicsSetLayouts); //Descriptor set IDs ptr (layout(set = #))
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		vkCheckError(vkCreatePipelineLayout(GraphicsHandler::get()->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout)) {
			COMPHILOG_CORE_FATAL("failed to create pipeline layout!");
			throw std::runtime_error("failed to create pipeline layout!");
		}

		COMPHILOG_CORE_INFO("created pipelineLayout successfully!");

		//Allocate DescriptorSets Once
		for (size_t i = 0; i < layoutSetsCount; i++)
		{
			uint descriptorSetsSize = graphicsSetLayouts[i].descriptorPools.size();
			graphicsSetLayouts[i].descriptorSets = std::vector<VkDescriptorSet>(descriptorSetsSize);
			for (size_t n = 0; n < descriptorSetsSize; n++)
			{
				VkDescriptorSetAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = graphicsSetLayouts[i].descriptorPools[n];
				allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
				allocInfo.pSetLayouts = &graphicsSetLayouts[i].descriptorSetLayout;

				vkCheckError(vkAllocateDescriptorSets(GraphicsHandler::get()->logicalDevice, &allocInfo, graphicsSetLayouts[i].descriptorSets.data())) {
					COMPHILOG_CORE_FATAL("failed to allocate descriptor sets!");
					return;
				}
			}
		}

		uint stageCount = config.pipelineLayoutConfiguration.shaderPrograms.size();
		std::vector<VkPipelineShaderStageCreateInfo> shaderStagesInfo = std::vector<VkPipelineShaderStageCreateInfo>(stageCount);
		for (size_t i = 0; i < stageCount; i++)
		{
			//Need to find a way to abstract Shader Modules after all ... DynamicCast It is !
			//Just have to guarantee that ComphiAPI Creates a full obj of type ShaderProgram
			ShaderProgram* _shaderProgram = static_cast<ShaderProgram*>(config.pipelineLayoutConfiguration.shaderPrograms[i]);

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
		
		configuration = config;
	}

	void GraphicsPipeline::updateDescriptorSet(ShaderResourceDescriptorType type, uint setID, uint descriptorID, void* data)
	{
		//https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkWriteDescriptorSet.html

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(1);
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = graphicsSetLayouts[setID].descriptorSets[descriptorID];
		descriptorWrites[0].dstBinding = descriptorID;
		descriptorWrites[0].dstArrayElement = 0;

		VkDescriptorBufferInfo bufferInfo{};
		VkDescriptorImageInfo imageInfo{};

		switch (type)
		{
		case ShaderResourceDescriptorType::UniformBuffer:
		{
			MemBuffer* buffer = static_cast<MemBuffer*>(data);
			bufferInfo.buffer = buffer[0].bufferObj;
			bufferInfo.offset = 0;
			bufferInfo.range = buffer[0].bufferSize;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;
			break;
		}

		case ShaderResourceDescriptorType::ImageBufferSampler:
		{
			ImageView* imageArr = static_cast<ImageView*>(data);
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = imageArr[0].imageView;
			imageInfo.sampler = imageArr[0].textureSampler;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pImageInfo = &imageInfo;
			break;
		}

		default:
			break;
		}

		vkUpdateDescriptorSets(GraphicsHandler::get()->logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		COMPHILOG_CORE_INFO("vkUpdateDescriptorSets success!");
	}

	void GraphicsPipeline::cleanUp()
	{
		for (size_t i = 0; i < descriptorPools.size(); i++)
		{
			COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorPool");
			vkDestroyDescriptorPool(Vulkan::GraphicsHandler::get()->logicalDevice, descriptorPools[i], nullptr);
		}
		descriptorPools.clear();

		for (size_t i = 0; i < descriptorSetLayouts.size(); i++)
		{
			COMPHILOG_CORE_INFO("vkDestroy Destroy descriptorSetLayout");
			vkDestroyDescriptorSetLayout(Vulkan::GraphicsHandler::get()->logicalDevice, descriptorSetLayouts[i], nullptr);
		}
		descriptorSetLayouts.clear();

		COMPHILOG_CORE_INFO("vkDestroy Destroy PipelineLayout");
		vkDestroyPipelineLayout(GraphicsHandler::get()->logicalDevice, pipelineLayout, nullptr);

		COMPHILOG_CORE_INFO("vkDestroy Destroy graphicsPipeline");
		vkDestroyPipeline(GraphicsHandler::get()->logicalDevice, pipelineObj, nullptr);

	}

	

}