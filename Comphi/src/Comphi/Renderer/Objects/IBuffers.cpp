#include "cphipch.h"
#include "IBuffers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///// OPENGL //////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Comphi::OpenGL {

    VertexBuffer::VertexBuffer(GLfloat* vertices, GLuint count)
    {
        //VERTEX BUFFER
        glGenVertexArrays(1, &arrayID);
        glBindVertexArray(arrayID);

        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, count, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * count, NULL);
    }

    void VertexBuffer::bind()
	{
		glBindVertexArray(arrayID);
	}

	void VertexBuffer::unbind()
	{
        glBindVertexArray(0);
	}

    IndexBuffer::IndexBuffer(GLuint* indices)
    {
        //INDEX BUFFER
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void IndexBuffer::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///// DIRECT_X ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Comphi::DirectX {

    VertexBuffer::VertexBuffer()
    {
    }

    IndexBuffer::IndexBuffer()
    {
    }
}