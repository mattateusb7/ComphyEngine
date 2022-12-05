#pragma once

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

	class IVertexBuffer
	{
	public:
		uint32_t i_vertexCount;
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