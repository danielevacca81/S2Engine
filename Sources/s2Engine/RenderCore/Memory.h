// Memory.h
//
#ifndef S2_RENDERCORE_MEMORY_H
#define S2_RENDERCORE_MEMORY_H

#include "s2Engine_API.h"

namespace s2 {
namespace RenderCore {

int S2ENGINE_API getGPUFreeMemory();
int S2ENGINE_API getGPUTotalMemory();
int S2ENGINE_API getGPUDedicatedTotalMemory();

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_MEMORY_H

