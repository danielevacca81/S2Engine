// ApplicationFlag.cpp
//
#include "ApplicationFlag.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

namespace s2 {

// ------------------------------------------------------------------------------------------------
void ConsoleFlag::activate()
{
	ApplicationFlag::activate();

#ifdef _WIN32

	AllocConsole();
	//freopen("CONOUT$", "w", stdout ); 

	FILE* fp;
	freopen_s( &fp, "CONOUT$", "w", stdout );

	std::cout.clear();
	std::wcout.clear();
#endif
}

}