#pragma once
#include "Comphi/Renderer/IShaderProgram.h"

namespace Comphi {

	//GraphicsPipelineConfiguration Options:
	//TODO: Abstract VulkanSpeecifications  (enums and stuff)

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
	
	struct AssemblySettings {
		PrimitiveTopologyType topologyType = TriangleList;
	};

	struct RasterizerSettings {
		PolygonMode polygonRenderMode;
		float lineWidth = 1.0f;
		CullingMode cullMode = BackCulling; //TODO: Toggle culling mode 
		FrontFaceOrientation frontFace = ClockWise;
		ColorBlendingModes blendingMode = AlphaBlend;
	};

	enum PixelFormat { 
		RGB_F32 = VK_FORMAT_R32G32B32_SFLOAT,
		RG_F32 = VK_FORMAT_R32G32_SFLOAT
	};

	enum vertexInputRate {
		Unique = VK_VERTEX_INPUT_RATE_VERTEX, //Temp name
		Instanced = VK_VERTEX_INPUT_RATE_INSTANCE
	};

	struct VertexBindingDescription {
		uint ID = 0;
		uint stride = 0;
		vertexInputRate inputRate = Unique;
	};

	struct VertexAttributeBinding {
		uint ID = 0;
		uint location = 0;
		PixelFormat format = RGB_F32;
		uint offset = 0; //offsetof(Vertex, color);
	};

	struct VertexBufferDescription {
		std::vector<VertexBindingDescription> bindingDescriptions;
		std::vector<VertexAttributeBinding> attributeDescriptionsBindings;
	};


	//PIPELINE DESCTIPTOR SETS & POOL
	enum ShaderStageFlags {
		FragmentStage = VK_SHADER_STAGE_FRAGMENT_BIT,
		VertexStage = VK_SHADER_STAGE_VERTEX_BIT
	};

	enum ShaderResourceDescriptorType {
		ImageBufferSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		UniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	};

	struct ShaderResourceDescriptorSet {
		//uint bindingID = 0; << arrayPos
		ShaderResourceDescriptorType type;
		uint count = 1; //Default = 1, Off = 0 | Resource Array Of Type x
		ShaderStageFlags flags;
	};

	//PIPELINE LAYOUT
	struct PipelineLayoutSet {
		std::vector<ShaderResourceDescriptorSet> shaderResourceDescriptorSets;
		//std::vector<pushConstants> //TODO: Add Later
	};

	struct PipelineLayoutConfiguration {
		std::vector<PipelineLayoutSet> layoutSets;
		std::vector<IShaderProgram*> shaderPrograms;
	};

	struct GraphicsPipelineConfiguration {
		AssemblySettings assemblySettings{};
		RasterizerSettings rasterizerSettings{};
		VertexBufferDescription vertexInputDescription{};
		PipelineLayoutConfiguration pipelineLayoutConfiguration{};

		//TODO: Add missing Configurations v v v 
		//pViewportState = &viewportState; //pDynamicState = &dynamicState;
		//pMultisampleState = &multisampling;
		//pDepthStencilState = &depthStencil; 
		
	};

	//struct GraphicsPipelineSetupData{
	//	VkViewport* viewport;
	//	VkRect2D* scissor;
	//}graphicsPipelineSetupData;

	class IGraphicsPipeline
	{
	public:
		IGraphicsPipeline() = default;
		~IGraphicsPipeline() = default;

		GraphicsPipelineConfiguration configuration;

	};
}
