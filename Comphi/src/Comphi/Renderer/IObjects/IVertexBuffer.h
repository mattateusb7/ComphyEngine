#pragma once

namespace Comphi {

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
	};

	typedef std::vector<Vertex> VertexArray;

	class IVertexBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}