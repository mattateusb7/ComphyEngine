#pragma once
#include <glm/glm.hpp>

namespace Comphi {

	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;
	};

	typedef std::vector<Vertex> VertexArray;

	class IVertexBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}