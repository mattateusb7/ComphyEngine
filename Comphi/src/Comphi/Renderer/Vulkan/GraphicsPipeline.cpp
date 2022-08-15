#include "cphipch.h"
#include "GraphicsPipeline.h"

namespace Comphi::Vulkan {

	GraphicsPipeline::GraphicsPipeline(GraphicsPipelineSetupData& graphicsPipelineSetupData) {
		this->graphicsPipelineSetupData = graphicsPipelineSetupData;
	}

	GraphicsPipeline::GraphicsPipeline() {}
	
	bool GraphicsPipeline::InitPipeline()
	{
		//VertexBufferDescription
		auto bindingDescription = VertexBuffer::getBindingDescription();
		auto attributeDescriptions = VertexBuffer::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		//Primitives
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		//Framebuffer/Viewport(Scissor)
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

		//Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE; //For Shadow mapping
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		//Multisampling 
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineDepthStencilStateCreateInfo depthStencilState{}; //NotinUse

		//ColorBlending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		{ //RGB Mixing 
			/*
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
			*/
		}
		{ //Alpha Blend: 
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


		//Pipelineslayout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(*logicalDevice.get(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create pipeline layout!");
			throw std::runtime_error("failed to create pipeline layout!");
		}

		COMPHILOG_CORE_INFO("created pipelineLayout successfully!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages.data();
		//pipelineInfo.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(*logicalDevice.get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
			COMPHILOG_CORE_FATAL("failed to create graphics pipeline!");
			throw std::runtime_error("failed to create graphics layout!");
		}
		COMPHILOG_CORE_INFO("created graphics pipeline successfully!");

		return true;
	}

	bool GraphicsPipeline::BindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram);
		logicalDevice = std::make_shared<VkDevice>(*_shaderProgram->logicalDevice);

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

	bool GraphicsPipeline::UnbindProgram(IShaderProgram& shaderProgram)
	{
		auto _shaderProgram = static_cast<ShaderProgram*>(&shaderProgram);

		auto it_shaderProgram = std::find(shaderPrograms.begin(), shaderPrograms.end(), _shaderProgram);
		if (it_shaderProgram != shaderPrograms.end()) {
			COMPHILOG_CORE_INFO("Destroyed ShaderModule!");
			vkDestroyShaderModule(*logicalDevice.get(), (*it_shaderProgram)->shaderModule, nullptr);
			shaderPrograms.erase(it_shaderProgram);
			return true;
		}
		return false;
	}

}