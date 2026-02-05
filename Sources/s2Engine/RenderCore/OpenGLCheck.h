// OpenGLCheck.h
//
#ifndef S2_RENDERCORE_OPENGLCHECK_H
#define S2_RENDERCORE_OPENGLCHECK_H

#include "s2Engine_API.h"

namespace s2 {
namespace RenderCore {

bool S2ENGINE_API isOpenGLDebugOutputEnabled();
void S2ENGINE_API enableOpenGLDebugOutput();
void S2ENGINE_API disableOpenGLDebugOutput();
void S2ENGINE_API checkOpenGLError( const char* file, unsigned int line );

#define glCheck checkOpenGLError(__FILE__, __LINE__)

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_OPENGLCHECK_H
