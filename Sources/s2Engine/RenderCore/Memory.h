// Memory.h
//
#ifndef RENDERCORE_MEMORY_H
#define RENDERCORE_MEMORY_H

#include "s2Engine_API.h"

namespace RenderCore {

int S2ENGINE_API getGPUFreeMemory();
int S2ENGINE_API getGPUTotalMemory();
int S2ENGINE_API getGPUDedicatedTotalMemory();

}
#endif // !RENDERCORE_MEMORY_H

