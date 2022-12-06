#include "cphipch.h"
#include "MeshObject.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Comphi::Vulkan {

	MeshObject::MeshObject(IFileRef& objFile, Material& material) //TODO: Add default Material
	{
		ParseObjFile(objFile);
		this->i_material = std::make_shared<Material>(material);
		initializeMVPMatrices();
		initializeMeshData();
	}

	MeshObject::MeshObject(VertexArray& vertices, IndexArray& indices, Material& material)
	{
		this->i_vertices = std::make_shared<VertexBuffer>(vertices);
		this->i_indices = std::make_shared<IndexBuffer>(indices);
		this->i_material = std::make_shared<Material>(material);	
		initializeMVPMatrices();
		initializeMeshData();
	}


	void MeshObject::ParseObjFile(IFileRef& objFile) {
		this->i_ModelFileOBJ = &objFile;

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFile.getFilePath().data())) {
			throw std::runtime_error(warn + err);
		}

		VertexArray vArray;
		IndexArray iArray;
		//Init Mesh Array Objects
		std::unordered_map<Vertex, uint32_t> uniqueVertices{}; //vertex,index

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1] //vulkan Flipped TexCoords
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				//Hash Vertex
				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vArray.size());
					vArray.push_back(vertex);
				}

				iArray.push_back(uniqueVertices[vertex]);
			}
		}

		//init Objects
		i_vertices = std::make_shared<Vulkan::VertexBuffer>(vArray);
		i_indices = std::make_shared<Vulkan::IndexBuffer>(iArray);
	}

	void MeshObject::updateMVP(uint currentImage)
	{
		void* data;
		vkMapMemory(*Vulkan::GraphicsHandler::get()->logicalDevice, static_cast<UniformBuffer>(i_MVP_UBOs[currentImage]).bufferMemory, 0, sizeof(i_MVP_UBOs), 0, &data);
		memcpy(data, &i_MVP_UBOs, sizeof(i_MVP_UBOs));
		vkUnmapMemory(*Vulkan::GraphicsHandler::get()->logicalDevice, static_cast<UniformBuffer>(i_MVP_UBOs[currentImage]).bufferMemory);
	}

	void MeshObject::bind(void* commandBuffer)
	{
		VkCommandBuffer vkCommand = (VkCommandBuffer)commandBuffer;

		//Bind VertexBuffers @0
		VkBuffer vertexBuffers[] = { ((VertexBuffer*)&i_vertices)->bufferObj };
		VkDeviceSize offsets[] = { 0 }; //batch render
		vkCmdBindVertexBuffers(vkCommand, 0, 1, vertexBuffers, offsets);

		//Bind IndexBuffers @1
		vkCmdBindIndexBuffer(vkCommand, static_cast<IndexBuffer*>(i_indices.get())->bufferObj, 0, VK_INDEX_TYPE_UINT32);

	}

	//API: 

	void MeshObject::initializeMVPMatrices()
	{
		int MAX_FRAMES_IN_FLIGHT = *Vulkan::GraphicsHandler::get()->MAX_FRAMES_IN_FLIGHT;
		i_MVP_UBOs.resize(MAX_FRAMES_IN_FLIGHT);
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			//initialize
			i_MVP_UBOs[i] = UniformBufferObject();
		}
	}

}