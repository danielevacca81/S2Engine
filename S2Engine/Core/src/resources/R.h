// R.h
// 
#ifndef R_ONCE
#define R_ONCE

#include "Core_API.h"

#include <string>

// ------------------------------------------------------------------------------------------------
namespace  R
{
	CORE_API void init();
	CORE_API void free();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	CORE_API std::string    string( const std::string &resourceName );
	CORE_API unsigned char *bytes ( const std::string &resourceName );
}
// ------------------------------------------------------------------------------------------------

#endif