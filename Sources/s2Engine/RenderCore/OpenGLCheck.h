// OpenGLCheck.h
//
#ifndef OPENGLCHECK_H
#define OPENGLCHECK_H

#include "s2Engine_API.h"

namespace RenderCore{

bool S2ENGINE_API isOpenGLDebugOutputEnabled();
void S2ENGINE_API enableOpenGLDebugOutput();
void S2ENGINE_API disableOpenGLDebugOutput();
void S2ENGINE_API checkOpenGLError( const char* file, unsigned int line );

#define glCheck checkOpenGLError(__FILE__, __LINE__)

}
#endif
