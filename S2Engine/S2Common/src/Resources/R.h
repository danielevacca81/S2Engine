// R.h
// 
#ifndef R_ONCE
#define R_ONCE

#include "S2CommonAPI.h"

#include <string>

// ------------------------------------------------------------------------------------------------
namespace  R
{
	S2COMMON_API void init();
	S2COMMON_API void free();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	S2COMMON_API std::string    string( const std::string &resourceName );
	S2COMMON_API unsigned char *bytes ( const std::string &resourceName );
}
// ------------------------------------------------------------------------------------------------

#endif