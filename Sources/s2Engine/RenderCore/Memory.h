// Memory.h
//
#ifndef RENDERCORE_MEMORY_H
#define RENDERCORE_MEMORY_H

#include "s2Engine_API.h"

namespace RenderCore {

int S2ENGINE_API getFreeMemory();
int S2ENGINE_API getTotalMemory();
int S2ENGINE_API getDedicatedTotalMemory();

}
#endif // !RENDERCORE_MEMORY_H

