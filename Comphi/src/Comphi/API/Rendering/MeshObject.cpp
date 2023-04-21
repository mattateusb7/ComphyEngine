#include "cphipch.h"
#include "MeshObject.h"
#include "Comphi/Renderer/Vulkan/Buffers/UniformBuffer.h"

namespace Comphi {

	MeshObject::MeshObject(IFileRef& modelFile, MeshBuffers& meshBuffers)
	{
		ModelLoader::ParseObj(modelFile, meshData);
		fillEmptyIndexArray(meshData.vertexData, meshData.indexData);
		initMeshBuffers(meshBuffers);
	}

	MeshObject::MeshObject(MeshData& meshData, MeshBuffers& meshBuffers)
	{
		fillEmptyIndexArray(meshData.vertexData, meshData.indexData);
		this->meshData = meshData;
		initMeshBuffers(meshBuffers);
	}

	MeshObject::MeshObject(VertexArray& vertexData, IndexArray& indexData, MeshBuffers& meshBuffers)
	{
		fillEmptyIndexArray(vertexData, indexData);
		meshData.vertexData = vertexData;
		meshData.indexData = indexData;
		initMeshBuffers(meshBuffers);
	}

	IndexArray& MeshObject::fillEmptyIndexArray(VertexArray& vertexData, IndexArray& indexData)
	{
		if (indexData.size() == 0) {
			for (size_t i = 0; i < vertexData.size(); i++)
			{
				indexData.push_back(indexData.size());
			}
		}
		return indexData;
	}

	void MeshObject::initMeshBuffers(MeshBuffers& meshBuffers)
	{
		this->meshBuffers = meshBuffers;
		
		this->meshBuffers.vertexBuffer = std::make_shared<Vulkan::UniformBuffer>(meshData.vertexData.data(), sizeof(meshData.vertexData[0]), meshData.vertexData.size(), BufferUsage::VertexBuffer);
		this->meshBuffers.vertexBuffer->updateBufferData(meshData.vertexData.data());
		
		this->meshBuffers.indexBuffer = std::make_shared<Vulkan::UniformBuffer>(meshData.indexData.data(), sizeof(meshData.indexData[0]), meshData.indexData.size(), BufferUsage::IndexBuffer);
		this->meshBuffers.indexBuffer->updateBufferData(meshData.indexData.data());
	}

}
