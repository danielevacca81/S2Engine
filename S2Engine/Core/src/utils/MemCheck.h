// MemCheck.h
//
#ifndef COMMON_MEMCHECK_H
#define COMMON_MEMCHECK_H

#include "Core_API.h"

#ifdef _WIN32
#include <cstdlib>
#include <crtdbg.h>
#include <cstring>


#define _CRTDBG_MAP_ALLOC
#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define new DEBUG_NEW
//#undef _CRTDBG_MAP_ALLOC
#endif

#endif

namespace MemCheck {

CORE_API void enableMemoryLeaksDetection();
CORE_API void dumpMemoryLeaks();

}

#endif