// OpenGL.h
//
#ifndef OPENGL_ONCE
#define OPENGL_ONCE

#include "Renderer_API.h"

//#include "glload/gl_2_1.h"
#include "GL/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>


#include <vector>
#include <string>


namespace OpenGL{

#ifndef NDEBUG
	#define glCheck			OpenGL::glCheckError(__FILE__, __LINE__)
	#define glValidate		glGetError() == 0
#else
	#define glCheck
	#define glValidate      true
#endif

void                     RENDERER_API glCheckError(const char* file, unsigned line);

}

#endif
