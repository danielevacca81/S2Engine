// Extensions.cpp
//
#include "Extensions.h"

#include "OpenGL.h"

#include <sstream>
#include <iostream>

// ------------------------------------------------------------------------------------------------
static bool supportMemoryQuery()
{
	return glewIsSupported( "GL_NVX_gpu_memory_info" );
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


namespace RenderSystem {

// ------------------------------------------------------------------------------------------------
// returns total dedicated memory in MB
int getDedicatedTotalMemory()
{
	GLint dedicatedMemKB = 0;

	if( supportMemoryQuery() )
		glGetIntegerv( GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicatedMemKB );

	glCheck;
	return dedicatedMemKB >> 10;
}


// ------------------------------------------------------------------------------------------------
// returns available free memory in MB
int getFreeMemory()
{
	GLint freeMemKB = 0;
	if( supportMemoryQuery() )
		glGetIntegerv( GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &freeMemKB );

	glCheck;
	return freeMemKB >> 10;
}

// ------------------------------------------------------------------------------------------------
// returns total available memory in MB
int getTotalMemory()
{
	GLint totalMemKB = 0;

	if( supportMemoryQuery() )
		glGetIntegerv( GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemKB );

	glCheck;
	return totalMemKB >> 10;
}

}