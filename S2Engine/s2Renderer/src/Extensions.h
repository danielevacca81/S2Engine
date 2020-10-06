// Extensions.h
//
#ifndef RENDERER_EXTENSIONS_H
#define RENDERER_EXTENSIONS_H

#include "s2Renderer_API.h"

#include <string>
#include <vector>

namespace Renderer {

std::vector<std::string> S2RENDERER_API extensions();
bool                     S2RENDERER_API initExtensions();
std::string              S2RENDERER_API contextInfo();
int                      S2RENDERER_API getFreeMemory();
int                      S2RENDERER_API getTotalMemory();
int                      S2RENDERER_API getDedicatedTotalMemory();

}
#endif // !RENDERER_EXTENSIONS_H

