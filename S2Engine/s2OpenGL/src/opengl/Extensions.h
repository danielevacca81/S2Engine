// Extensions.h
//
#ifndef RENDERER_EXTENSIONS_H
#define RENDERER_EXTENSIONS_H

#include "s2OpenGL_API.h"

#include <string>
#include <vector>


namespace OpenGL
{

std::vector<std::string> S2OPENGL_API extensions();
bool                     S2OPENGL_API initExtensions();
std::string              S2OPENGL_API contextInfo();
bool                     S2OPENGL_API supportMemoryQuery();
int                      S2OPENGL_API getFreeMemory();
int                      S2OPENGL_API getTotalMemory();
int                      S2OPENGL_API getDedicatedTotalMemory();

}

#endif // !RENDERER_EXTENSIONS_H

