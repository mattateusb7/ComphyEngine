#pragma once
#include "../Objects/UniformBuffer.h"
#include "../Objects/ShaderProgram.h"
#include "ImageView.h"

namespace Comphi::Vulkan {

	//GraphicsPipelineConfiguration Options:

	enum ColorBlendingModes {
		AlphaBlend = 0,
		RGBMixing = 1
	};

	enum PrimitiveTopologyType {
		TriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		LineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	};

	enum PolygonMode { //TODO: Toggle Wireframe
		Wireframe = VK_POLYGON_MODE_LINE,
		PolygonFill = VK_POLYGON_MODE_FILL
	};

	enum CullingMode {
		BackCulling = VK_CULL_MODE_BACK_BIT,
		NoCulling = VK_CULL_MODE_NONE
	};

	enum FrontFaceOrientation {
		ClockWise = VK_FRONT_FACE_COUNTER_CLOCKWISE
	};

	struct RasterizerSettings {
		PolygonMode polygonRenderMode;
		float lineWidth = 1.0f;
		CullingMode cullMode = BackCulling; //TODO: Toggle culling mode 
		FrontFaceOrientation frontFace = ClockWise;
		ColorBlendingModes blendingMode = AlphaBlend;
	};

	enum ColorFormat {
		RGB_F32 = VK_FORMAT_R32G32B32_SFLOAT,
		RG_F32 = VK_FORMAT_R32G32_SFLOAT
	};

	enum vertexInputRate {
		Unique = VK_VERTEX_INPUT_RATE_VERTEX, //Temp name
		Instanced = VK_VERTEX_INPUT_RATE_INSTANCE
	};

	struct BindingDescription {
		uint ID = 0;
		uint stride = 0;
		vertexInputRate inputRate = Unique;
	};

	struct AttributeBinding {
		uint ID = 0;
		uint location = 0;
		ColorFormat format = RGB_F32;
		uint offset = 0; //offsetof(Vertex, color);
	};

	struct VertexBufferDescription {
		std::vector<BindingDescription> bindingDescriptions;
		std::vector<AttributeBinding> attributeDescriptionsBindings;
	};

	struct GraphicsPipelineConfiguration {
		PrimitiveTopologyType topologyType = TriangleList;
		RasterizerSettings rasterizerSettings{};
		VertexBufferDescription vertexInputDescription{};

		//pInputAssemblyState = &inputAssembly;
		//pViewportState = &viewportState;
		//pRasterizationState = &rasterizer;
		//pMultisampleState = &multisampling;
		//pDepthStencilState = &depthStencil; // Optional
		//pColorBlendState = &colorBlending;
		//pDynamicState = &dynamicState;

		//std::vector<VkPipelineShaderStageCreateInfo> shaderPrograms; //TODO Send Shader Program refs ?
		ShaderPrograms shaderPrograms; // CONTINUE HERE !

	};

	class GraphicsPipeline
	{
	public:

		//struct GraphicsPipelineSetupData{
		//	VkViewport* viewport;
		//	VkRect2D* scissor;
		//}graphicsPipelineSetupData;

		GraphicsPipeline() = default;
		void initialize(GraphicsPipelineConfiguration config);
		VkPipelineLayout pipelineLayout;
		VkPipeline pipelineObj;

		//std::vector<VkDescriptorSet> descriptorSets;
		//VkDescriptorPool descriptorPoolObj;

		//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts
		//Or just dynamic Binding for construction of those structs
		//void sendDescriptorSet(std::vector<ITexture*>& textures, std::vector<UniformBuffer>& MVP_ubos);
		
		void cleanUp();
	
	protected:
		//void createDescriptorPool();
		//void createDescriptorSetLayout();
		////void createPipelineLayout();
		//VkDescriptorSetLayout descriptorSetLayout;
	};

}