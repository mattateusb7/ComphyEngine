#pragma once
#include "Comphi/Renderer/IObjects/IVertexBuffer.h"
#include <glad/glad.h>

namespace Comphi::OpenGL {
	class VertexBuffer : public IVertexBuffer
	{
	public:
		VertexBuffer(const GLfloat& vertices, const GLuint& count);
		uint VAO, VBO;

		virtual void bind() override;
		virtual void unbind() override;
	};
}

