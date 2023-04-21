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
		
		//TODO : Test attribute offset of vertex data layout in shader
		// Calculate the offset of the member within the Vertex struct
		const T* temp = nullptr;
		const uintptr_t memberOffset = reinterpret_cast<uintptr_t>(&(temp->*member));
		const uintptr_t structOffset = reinterpret_cast<uintptr_t>(temp);
		vertexAttribute.offset = static_cast<uint32_t>(memberOffset - structOffset);

		//T temp;
		//vertexAttribute.offset = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&std::mem_fn(member)(temp)));
		//vertexAttribute.offset = static_cast<uint32_t>(reinterpret_cast<size_t>(&(((T*)nullptr)->*member)));

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

	/*
	In Vulkan API, each descriptor set layout specifies the type of resources that can be bound 
	to the corresponding descriptor set. 
	Therefore, a layout set ID can only bind resources of a specific DescriptorSetResourceType,
	which is specified in the descriptor set layout that corresponds to that ID.

	For example, if a descriptor set layout specifies that it can only bind UniformBufferData 
	resources, then you can only bind UniformBufferData to the descriptor set that corresponds 
	to that layout set ID. If you want to bind ImageBufferSampler resources to a different 
	descriptor set, you would need to use a different layout set ID that corresponds to 
	a descriptor set layout that allows for ImageBufferSampler resources.
	*/

	void Material::createShaderResourceLayoutSetDescriptorSetBinding(LayoutSetUpdateFrequency layoutSetID, uint bindingID, uint resourceDescriptorSetCount, DescriptorSetResourceType type, ShaderStageFlag shaderStage)
	{
	
		//dynamic increase layout sets count
		auto& layoutSets = configuration.pipelineLayoutConfiguration.layoutSets;
		if (layoutSetID + 1 > layoutSets.size()) {
			layoutSets.resize(layoutSetID+1);
		}

		layoutSets[layoutSetID].updateFrequency = layoutSetID;

		auto& descriptorSetBindings = layoutSets[layoutSetID].shaderResourceDescriptorSetBindings;

		//dynamic increase binding sets count
		if (bindingID + 1 > descriptorSetBindings.size()) {
			descriptorSetBindings.resize(bindingID + 1);
		}

		DescriptorSetBinding binding;
		binding.resourceCount = resourceDescriptorSetCount;
		binding.resourceType = type;
		binding.shaderStage = shaderStage;

		descriptorSetBindings[bindingID] = binding;
		
	}

	


}