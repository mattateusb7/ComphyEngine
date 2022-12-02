#include "cphipch.h"
#include "MeshObject.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

namespace Comphi {

	MeshObject::MeshObject(IFileRef& objFile, Material& material) //TODO: Add default Material
	{
		initialize(objFile, material);
	}

	void MeshObject::initialize(IFileRef& objFile, Material& material)
	{
		ParseObjFile(objFile);
		this->material = std::make_shared<Material>(material);
	}

	MeshObject::MeshObject(VertexArray& vertices, IndexArray& indices, Material& material)
	{
		initialize(vertices, indices, material);
	}

	void MeshObject::initialize(VertexArray& vertices, IndexArray& indices, Material& material) 
	{
		this->vertices = std::make_shared<Vulkan::VertexBuffer>(vertices);
		this->indices = std::make_shared<Vulkan::IndexBuffer>(indices);
		this->material = std::make_shared<Material>(material);
	}

	void MeshObject::ParseObjFile(IFileRef& objFile) {
		this->objFile = &objFile;

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
		vertices = std::make_shared<Vulkan::VertexBuffer>(vArray);
		indices = std::make_shared<Vulkan::IndexBuffer>(iArray);
	}




}