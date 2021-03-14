#include "cphipch.h"
#include "GLError.h"

using namespace std;
bool _b_gl_debug_enabled = true;

bool _check_gl_error(const char *file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		string error;

		switch (err)
		{
		case GL_INVALID_OPERATION:							error = "INVALID_OPERATION";							break;
		case GL_INVALID_ENUM:								error = "INVALID_ENUM";									break;
		case GL_INVALID_VALUE:								error = "INVALID_VALUE";								break;
		case GL_OUT_OF_MEMORY:								error = "OUT_OF_MEMORY";								break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:				error = "INVALID_FRAMEBUFFER_OPERATION : ";				break;
		//case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	error = "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";	break;
		//case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			error = "FRAMEBUFFER_INCOMPLETE_ATTACHMENT";			break;
		//case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT :		error = "FRAMEBUFFER_INCOMPLETE_DIMENSIONS";			break;
		//case GL_FRAMEBUFFER_UNSUPPORTED:					error = "FRAMEBUFFER_UNSUPPORTED";						break;
		
		}

		if (_b_gl_debug_enabled)
			cout << "GL_" << error.c_str() << " - " << file << ":" << line << endl;
		err = glGetError();
		return true;
	}
	return false;
}

void _toggle_gl_debug(bool mode)
{
	_b_gl_debug_enabled = mode;
}
