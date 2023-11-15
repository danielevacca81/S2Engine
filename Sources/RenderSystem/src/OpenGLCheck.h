// OpenGLCheck.h
//
#ifndef OPENGLCHECK_ONCE
#define OPENGLCHECK_ONCE

#include "RenderSystem_API.h"

namespace RenderSystem{

bool RENDERSYSTEM_API isOpenGLDebugOutputEnabled();
void RENDERSYSTEM_API enableOpenGLDebugOutput();
void RENDERSYSTEM_API disableOpenGLDebugOutput();
void RENDERSYSTEM_API checkOpenGLError( const char* file, unsigned int line );

#define glCheck checkOpenGLError(__FILE__, __LINE__)

}
#endif
