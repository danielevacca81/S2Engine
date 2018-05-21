// OpenGL.h
//
#ifndef OPENGL_ONCE
#define OPENGL_ONCE

#include "S2RendererAPI.h"

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

std::vector<std::string> S2RENDERER_API glExtensions();
bool                     S2RENDERER_API glInitExtensions();
std::string              S2RENDERER_API glContextInfo();
void                     S2RENDERER_API glCheckError(const char* file, unsigned line);
bool                     S2RENDERER_API glSupportMemoryQuery();
int                      S2RENDERER_API glGetFreeMemory();
int                      S2RENDERER_API glGetTotalMemory();
int                      S2RENDERER_API glGetDedicatedTotalMemory();

}

#endif
