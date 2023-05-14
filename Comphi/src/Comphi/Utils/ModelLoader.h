#pragma once
#include "Comphi/Platform/IFileRef.h"

namespace Comphi {

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	};

	typedef std::vector<Vertex> VertexArray;

	typedef uint32_t Index;
	typedef std::vector<Index> IndexArray;

	struct MeshData {
		VertexArray vertexData;
		IndexArray indexData;
	};


	class ModelLoader
	{
	public:
		static void ParseObj(IFileRef& objFile, MeshData& outData);
	private: 
		ModelLoader() = default;
		~ModelLoader() = default;
	};

}

namespace std {
	template<> struct hash<Comphi::Vertex> {
		size_t operator()(Comphi::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}