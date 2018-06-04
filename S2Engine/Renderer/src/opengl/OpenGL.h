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

std::vector<std::string> RENDERER_API glExtensions();
bool                     RENDERER_API glInitExtensions();
std::string              RENDERER_API glContextInfo();
void                     RENDERER_API glCheckError(const char* file, unsigned line);
bool                     RENDERER_API glSupportMemoryQuery();
int                      RENDERER_API glGetFreeMemory();
int                      RENDERER_API glGetTotalMemory();
int                      RENDERER_API glGetDedicatedTotalMemory();

}

#endif
