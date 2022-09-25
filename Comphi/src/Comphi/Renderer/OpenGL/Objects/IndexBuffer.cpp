#include "cphipch.h"
#include "IndexBuffer.h"

namespace Comphi::OpenGL {

    IndexBuffer::IndexBuffer(const IndexArray& indices)
    {
        //INDEX BUFFER
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
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