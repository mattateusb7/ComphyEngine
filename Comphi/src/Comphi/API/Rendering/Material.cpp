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

		T temp;
		vertexAttribute.offset = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&std::mem_fn(member)(temp)));
		
		//auto memberValue = std::mem_fn(member)(T{});
		//vertexAttribute.offset = offsetof(T, memberValue);
		
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

	void Material::addShaderResource(uint layoutSetID, uint dataObjectArrayCount, ShaderResourceDescriptorType type, ShaderStageFlag shaderStage)
	{
		DescriptorSetBinding binding;
		binding.dataObjectArrayCount = dataObjectArrayCount;
		binding.resourceType = type;
		binding.shaderStage = shaderStage;

		if (layoutSetID+1 > configuration.pipelineLayoutConfiguration.layoutSets.size())
			configuration.pipelineLayoutConfiguration.layoutSets.resize(layoutSetID+1);

		configuration.pipelineLayoutConfiguration.layoutSets[layoutSetID].shaderResourceDescriptorSets.push_back(binding);

		
	}

	


}