#include "cphipch.h"
#include "Material.h"
#include "Comphi/Utils/ModelLoader.h"

namespace Comphi {

	template<typename T>
	inline void Material::addVertexBindingID(uint ID, vertexInputRate inputRate)
	{
		VertexBufferBindingDescription vertexBufferLayout;
		vertexBufferLayout.bufferBindingID = ID;
		vertexBufferLayout.vertexStride = sizeof(T);
		vertexBufferLayout.inputRate = inputRate;
		configuration.vertexInputLayoutConfiguration.vertexBufferBindingDescriptors.push_back(vertexBufferLayout);
	}

	template<typename T, typename M>
	inline void Material::addVertexAttribute(uint layoutBindingID, uint layoutLocationID, const M T::* member, PixelFormat format)
	{
		VertexAttributeBindingDescription vertexAttribute;
		vertexAttribute.bufferBindingID = layoutBindingID;
		vertexAttribute.shaderLocationID = layoutLocationID;
		vertexAttribute.format = format;
		vertexAttribute.offset = offsetof(T, member);
		configuration.vertexInputLayoutConfiguration.vertexAttributeFormatDescriptors.push_back(vertexAttribute);
	}

	void Material::addDefaultVertexBindingDescription()
	{
		addVertexBindingID<Vertex>(0);
		addVertexAttribute(0,0,&Vertex::pos,PixelFormat::RGB_F32);
		addVertexAttribute(0,1,&Vertex::color,PixelFormat::RGB_F32);
		addVertexAttribute(0,2,&Vertex::texCoord,PixelFormat::RG_F32);
	}

	void Material::addShader(ShaderObjectPtr shaderObject)
	{
		configuration.pipelineLayoutConfiguration.shaderPrograms.push_back(shaderObject.get());
	}

	void Material::addLayoutSet(ResourceUpdateFrequency updateFrequency)
	{
		PipelineLayoutSet layout;
		layout.updateFrequency = updateFrequency;
		configuration.pipelineLayoutConfiguration.layoutSets.push_back(layout);
	}

	void Material::addShaderResourceToLayoutset(uint layoutSetID, uint dataObjectArrayCount, ShaderResourceDescriptorType type, ShaderStageFlag shaderStage)
	{
		DescriptorSetBinding binding;
		binding.dataObjectArrayCount = dataObjectArrayCount;
		binding.resourceType = type;
		binding.shaderStage = shaderStage;

		configuration.pipelineLayoutConfiguration.layoutSets[layoutSetID].shaderResourceDescriptorSets.push_back(binding);

		
	}

	


}