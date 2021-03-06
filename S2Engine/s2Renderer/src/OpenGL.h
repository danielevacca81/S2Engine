// OpenGL.h
//
#ifndef OPENGL_ONCE
#define OPENGL_ONCE

#include "s2Renderer_API.h"


#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>


namespace s2 {
namespace Renderer{

#ifndef NDEBUG
	#define glCheck			glCheckError(__FILE__, __LINE__)
	#define glValidate		glGetError() == 0
#else
	#define glCheck
	#define glValidate      true
#endif

void                     S2RENDERER_API glCheckError(const char* file, unsigned line);

}
}

#endif
