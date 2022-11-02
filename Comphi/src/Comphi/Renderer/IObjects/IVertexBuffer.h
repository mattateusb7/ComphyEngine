#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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
		virtual void bind() = 0;
		virtual void unbind() = 0;
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