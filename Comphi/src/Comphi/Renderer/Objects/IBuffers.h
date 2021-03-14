#pragma once 
#include <glad/glad.h>

namespace Comphi {

	class IBuffer 
	{
	};

	class IIndexBuffer : public IBuffer 
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	class IVertexBuffer : public IBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// OPENGL //////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace OpenGL {
		class VertexBuffer : public IVertexBuffer
		{
		public:
			VertexBuffer(GLfloat* vertices, GLuint count);
			uint arrayID;
			uint bufferID;

			virtual void bind() override;
			virtual void unbind() override;
		};

		class IndexBuffer : public IIndexBuffer
		{
		public:
			IndexBuffer(GLuint* indices);
			uint ID;

			virtual void bind() override;
			virtual void unbind() override;
		};

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// DIRECT_X ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace DirectX {

		class VertexBuffer : public IVertexBuffer
		{
		public:
			VertexBuffer();
			
			virtual void bind() override {};
			virtual void unbind() override {};
		};

		class IndexBuffer : public IIndexBuffer
		{
		public:
			IndexBuffer();

			virtual void bind() override {};
			virtual void unbind() override {};
		};

	}
}


