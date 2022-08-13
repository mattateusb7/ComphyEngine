#pragma once
#include "Comphi/Renderer/IObjects/IIndexBuffer.h"
#include <glad/glad.h>

namespace Comphi::OpenGL {
	class IndexBuffer : public IIndexBuffer
	{
	public:
		IndexBuffer(const GLuint& indices);
		uint ID;

		virtual void bind() override;
		virtual void unbind() override;
	};
}