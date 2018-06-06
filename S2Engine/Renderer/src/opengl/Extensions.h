// Extensions.h
//
#ifndef RENDERER_EXTENSIONS_H
#define RENDERER_EXTENSIONS_H

#include "Renderer_API.h"

#include <string>
#include <vector>


namespace OpenGL
{

std::vector<std::string> RENDERER_API extensions();
bool                     RENDERER_API initExtensions();
std::string              RENDERER_API contextInfo();
bool                     RENDERER_API supportMemoryQuery();
int                      RENDERER_API getFreeMemory();
int                      RENDERER_API getTotalMemory();
int                      RENDERER_API getDedicatedTotalMemory();

}

#endif // !RENDERER_EXTENSIONS_H

