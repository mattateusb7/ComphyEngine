#pragma once

#include "Comphi/Renderer/Vulkan/Objects/VertexBuffer.h"
#include "Comphi/Renderer/Vulkan/Objects/IndexBuffer.h"
#include "Comphi/Renderer/Vulkan/Objects/ImageView.h" 
#include "Comphi/Renderer/Vulkan/Objects/UniformBuffer.h"

namespace Comphi { //(Comphi namepsace objects should use platform & renderer independent interfaces)
	class MeshObject 
	{
	public:
		MeshObject() = default;
		MeshObject(std::string objFile, std::string textureFile = "");
		MeshObject(VertexArray& vertices, IndexArray& indices, std::string textureFile = "");

		void initialize(std::string objFile, std::string textureFile);
		void initialize(VertexArray& vertices, IndexArray& indices, std::string textureFile);

		Windows::FileRef objFile;
		std::shared_ptr<Vulkan::VertexBuffer> vertices;
		std::shared_ptr<Vulkan::IndexBuffer> indices;
		
		Windows::FileRef textureFile;					//move to ShaderObj
		std::shared_ptr<Vulkan::ImageView> texture;		//move to ShaderObj

		std::vector<std::shared_ptr<Vulkan::UniformBuffer>> ubos; //Move to TransformObj and figure out why array ?

	protected:
		void ParseObjFile(std::string objFile);
		void initTextureSampler(std::string textureFile);
		void initUBO();
	};
}

