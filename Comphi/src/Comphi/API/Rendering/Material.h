#pragma once

#include "Comphi/Renderer/IGraphicsPipeline.h"
#include "Comphi/API/Rendering/ShaderObject.h"
#include "Comphi/API/Rendering/MaterialInstance.h"

namespace Comphi {

	class Material : IGraphicsPipeline
	{
	public:
		Material() = default;
		
		void addDefaultVertexBindingDescription();
		void addDefaultLayoutSet();
		
		template<typename T>
		inline void addVertexBindingID(uint ID, vertexInputRate inputRate = PerVertex);

		template<typename T, typename M>
		inline void addVertexAttribute(uint layoutBindingID, uint layoutLocationID, const M T::* member, PixelFormat format = R_F32);

		void addShader(ShaderObjectPtr shaderObject);
		void addLayoutSet(ResourceUpdateFrequency updateFrequency = ResourceUpdateFrequency::PerMaterialInstance);
		void addShaderResourceToLayoutset(uint layoutSetID, uint dataObjectArrayCount, ShaderResourceDescriptorType type = UniformBuffer, ShaderStageFlag shaderStage = ShaderStageFlag::AllGraphics);

		virtual void initialize() override;
		~Material() = default;
	};

	//TODO : Automate pipelineCustomization;

	typedef std::shared_ptr<Material> MaterialPtr;



}

