// OpenGL.h
//
#ifndef OPENGL_ONCE
#define OPENGL_ONCE

#include "s2OpenGL_API.h"

//#include "glload/gl_2_1.h"
#include "GL/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>


#include <vector>
#include <string>

namespace s2 {

namespace OpenGL{

#ifndef NDEBUG
	#define glCheck			OpenGL::glCheckError(__FILE__, __LINE__)
	#define glValidate		glGetError() == 0
#else
	#define glCheck
	#define glValidate      true
#endif

void                     S2OPENGL_API glCheckError(const char* file, unsigned line);

}
}

#endif