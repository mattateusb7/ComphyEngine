#include "cphipch.h"
#include "IBuffers.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///// OPENGL //////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Comphi::OpenGL {

    VertexBuffer::VertexBuffer(const GLfloat& vertices, const GLuint& count)
    {
        GLsizei elemsPerVertex = 3;
        GLsizei length = (elemsPerVertex * count);
        GLsizei floatSize = sizeof(GLfloat);
        GLsizei VAOsize = (floatSize * length);
        GLsizei VAOstride = (floatSize * elemsPerVertex);

        /**Vertex Array Object Attributes**/
        const GLsizei Pos_atrib = 0;

        /**VERTEX BUFFER OBJ**/
        glCreateBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VAOsize, &vertices, GL_STATIC_DRAW);

        /**VERTEX BUFFER OBJECT**/
        glCreateVertexArrays(1, &VAO);
        glEnableVertexArrayAttrib(VAO, Pos_atrib);
        //glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::Norm_atrib);
        //glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::Tangent_atrib);
        //glEnableVertexArrayAttrib(vao, _NL::Core::GLSL_AU::TexC_atrib);

        //Vertex Array Format
        glVertexArrayAttribBinding(VAO, Pos_atrib, 0);
        glVertexArrayAttribFormat(VAO, Pos_atrib, 3, GL_FLOAT, GL_FALSE, floatSize * 0);

        //glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::Norm_atrib, 0);
        //glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::Norm_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 3);
        //
        //glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::Tangent_atrib, 0);
        //glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::Tangent_atrib, 3, GL_FLOAT, GL_FALSE, foatSize * 6);
        //
        //glVertexArrayAttribBinding(vao, _NL::Core::GLSL_AU::TexC_atrib, 0);
        //glVertexArrayAttribFormat(vao, _NL::Core::GLSL_AU::TexC_atrib, 2, GL_FLOAT, GL_FALSE, foatSize * 9);

        /**Configure : link Buffers to Vertex Array **/
        glVertexArrayVertexBuffer(VAO, 0, VBO, 0, VAOstride);
    }

    void VertexBuffer::bind()
	{   
		glBindVertexArray(VAO);
	}

	void VertexBuffer::unbind()
	{
        glBindVertexArray(0);
	}

    IndexBuffer::IndexBuffer(const GLuint& indices)
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