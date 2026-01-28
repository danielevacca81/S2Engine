// OpenGLCheck.h
//
#ifndef OPENGLCHECK_H
#define OPENGLCHECK_H

#include "RenderCore_API.h"

namespace RenderCore{

bool RENDERCORE_API isOpenGLDebugOutputEnabled();
void RENDERCORE_API enableOpenGLDebugOutput();
void RENDERCORE_API disableOpenGLDebugOutput();
void RENDERCORE_API checkOpenGLError( const char* file, unsigned int line );

#define glCheck checkOpenGLError(__FILE__, __LINE__)

}
#endif
