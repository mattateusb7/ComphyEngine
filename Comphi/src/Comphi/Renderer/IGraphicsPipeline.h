#pragma once
#include "Comphi/Renderer/IShaderProgram.h"
#include "Comphi/Renderer/IUniformBuffer.h"

namespace Comphi {

	//TODO: verify if this is the best way to Abstract VulkanSpeecifications (enums and stuff)

	//GraphicsPipelineConfiguration Options:

	enum ColorBlendingModes {
		AlphaBlend = 0,
		RGBMixing = 1
	};

	enum PrimitiveTopologyType {
		TriangleList = 3,//VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		LineList = 1//VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	};

	enum PolygonMode { //TODO: Toggle Wireframe
		Wireframe = 1,//VK_POLYGON_MODE_LINE
		PolygonFill = 0//VK_POLYGON_MODE_FILL
	};

	enum CullingMode {
		BackCulling = 0x00000002,//VK_CULL_MODE_BACK_BIT,
		NoCulling = 0 //VK_CULL_MODE_NONE
	};

	enum FrontFaceOrientation {
		ClockWise = 0,//VK_FRONT_FACE_COUNTER_CLOCKWISE
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
		RGBA_F32 = 109, //VK_FORMAT_R32G32B32A32_SFLOAT
		RGB_F32	 = 106, //VK_FORMAT_R32G32B32_SFLOAT
		RG_F32	 = 103, //VK_FORMAT_R32G32_SFLOAT
		R_F32	 = 100	//VK_FORMAT_R32_SFLOAT 
	};

	enum vertexInputRate {
		PerVertex	= 0, //VK_VERTEX_INPUT_RATE_VERTEX,
		PerInstance	= 1	 //VK_VERTEX_INPUT_RATE_INSTANCE
	};

	struct VertexBufferBindingDescription {
		uint bufferBindingID = 0; //buffer ID
		uint vertexStride = 0; //sizeof(Vertex) the size of each vertex in bytes
		vertexInputRate inputRate = PerVertex;
	};

	struct VertexAttributeBindingDescription {
		uint bufferBindingID = 0; //which buffer ID to read from
		uint shaderLocationID = 0; //shader variable location
		PixelFormat format = RGB_F32;
		uint offset = 0; //offsetof(Vertex, color);
	};

	struct VertexBuffersLayoutConfiguration {
		std::vector<VertexBufferBindingDescription> vertexBufferBindingDescriptors;
		std::vector<VertexAttributeBindingDescription> vertexAttributeFormatDescriptors;
	};

	//PIPELINE DESCTIPTOR SETS & POOL

	enum ShaderResourceDescriptorType {
		ImageBufferSampler	= 1, //VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
		UniformBuffer		= 6 //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	};

	enum ResourceUpdateFrequency {
		GlobalData			= 0,
		PerScene			= 1,
		PerMaterialInstance = 2,
		PerMeshObject		= 3,
		PerEntity			= 4
	};

	//bindingID = arrayPos?
	struct DescriptorSetBinding {
		uint dataObjectArrayCount = 1; //Default = 1, Off = 0 | Resource Array Of Type x
		ShaderResourceDescriptorType resourceType;
		ShaderStageFlag shaderStage;
	};

	//PIPELINE LAYOUT
	struct PipelineLayoutSet {
		std::vector<DescriptorSetBinding> shaderResourceDescriptorSets;
		ResourceUpdateFrequency updateFrequency; //affects rendering Loop step
		//std::vector<pushConstants> //TODO: Add Later
	};

	struct PipelineLayoutConfiguration {
		std::vector<PipelineLayoutSet> layoutSets;
		std::vector<IShaderProgram*> shaderPrograms;
	};

	struct GraphicsPipelineConfiguration {
		AssemblySettings assemblySettings{};
		RasterizerSettings rasterizerSettings{};
		VertexBuffersLayoutConfiguration vertexInputLayoutConfiguration{};
		PipelineLayoutConfiguration pipelineLayoutConfiguration{};

		//TODO: Add missing Configurations v v v 
		//pViewportState = &viewportState; == //pDynamicState = &dynamicState;
		//pMultisampleState = &multisampling;
		//pDepthStencilState = &depthStencil; 
		
	};

	//struct GraphicsPipelineSetupData{
	//	VkViewport* viewport;
	//	VkRect2D* scissor;
	//}graphicsPipelineSetupData;

	class IGraphicsPipeline : public IObject
	{
	public:
		GraphicsPipelineConfiguration configuration;
		virtual void initialize() = 0;
		//virtual void updateShaderResource() {};
	};

}

/*
SCENE GRAPH 



Orb(GameObject){
	Scene*
	Trasform{
		ParentTransform*
	}
}

Materials[]
{
	Pipeline*
	data*
}


Data[]
{
	Pipeline*
	data*
}

Instance:Orb[]
{
	
	
	
}

Camera(EyeObserver){
	SceneFilter*
}

//GAME SCENE GRAPH

Scenes[] 
{
	Instances[]
	{
		Materials[]
		{
			Pipeline*
			data*
		}

	}

}

//RENDER ENGINE: 

Pipelines[] (Global)
{
	Scenes[] (PerScene)
	{
		RenderInstances[] (sorted By Material)
		{
			Materials[]{
				Pipeline*
				Shaders[]
				Data[]{
				}
			}
			
		}

	}
}


*/