// Extensions.h
//
#ifndef RENDERER_EXTENSIONS_H
#define RENDERER_EXTENSIONS_H

#include "RenderSystem_API.h"

#include <string>
#include <vector>

namespace RenderSystem {

int                      RENDERSYSTEM_API getFreeMemory();
int                      RENDERSYSTEM_API getTotalMemory();
int                      RENDERSYSTEM_API getDedicatedTotalMemory();

}
#endif // !RENDERER_EXTENSIONS_H

