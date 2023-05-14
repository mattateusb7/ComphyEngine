#pragma once
#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "Comphi/API/Rendering/ShaderObject.h"

namespace Comphi {

	class Material : public IGraphicsPipeline
	{
	public:

		Material(IGraphicsPipelinePtr& pipeline) : IGraphicsPipeline(*pipeline.get()) {
			this->pipeline = pipeline;
		}
		
		void addDefaultVertexBindingDescription();
		
		template<typename T>
		inline void addVertexBindingID(uint ID, vertexInputRate inputRate = PerVertex);

		template<typename T, typename M>
		inline void addVertexAttribute(uint layoutBindingID, uint layoutLocationID, const M T::* member, PixelFormat format = R_F32);

		void addShader(ShaderObjectPtr shaderObject);
		void createShaderResourceLayoutSetDescriptorSetBinding(LayoutSetUpdateFrequency layoutSetID, uint bindingID, uint resourceDescriptorSetCount, DescriptorSetResourceType type = UniformBufferData, ShaderStageFlag shaderStage = ShaderStageFlag::AllGraphics);

		virtual void initialize() override {
			pipeline->configuration = configuration;
			pipeline->initialize(); 
			configuration = pipeline->configuration;
		};

		IGraphicsPipelinePtr getIPipelinePtr() {
			return pipeline;
		}
	private:
		IGraphicsPipelinePtr pipeline;
	};

	typedef std::shared_ptr<Material> MaterialPtr;

}

//template<> struct std::hash<Comphi::MaterialPtr> {
//	size_t operator()(Comphi::MaterialPtr const& material) const {
//		return material->UID;
//	}
//};

