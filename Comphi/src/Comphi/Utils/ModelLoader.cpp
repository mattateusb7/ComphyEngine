#pragma once
#include"cphipch.h"
#include "ModelLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Comphi {

	void ModelLoader::ParseObj(IFileRef& objFile, MeshData& outData) {
		//TODO: Add mulithreading safety with mutex

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFile.getFilePath().data())) {
			throw std::runtime_error(warn + err);
		}

		//Init Mesh Array Objects
		std::unordered_map<Vertex, Index> uniqueVertices{};

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
					uniqueVertices[vertex] = static_cast<uint32_t>(outData.vertexData.size()); //currIndex
					outData.vertexData.push_back(vertex);
				}

				outData.indexData.push_back(uniqueVertices[vertex]);
			}
		}

	}

}