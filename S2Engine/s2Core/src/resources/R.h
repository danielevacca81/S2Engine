// R.h
// 
#ifndef R_ONCE
#define R_ONCE

#include "s2Core_API.h"

#include <string>

namespace s2 {
namespace  R {

S2CORE_API void init();
S2CORE_API void free();

/************************************************************************/
/*                                                                      */
/************************************************************************/
S2CORE_API std::string    string( const std::string &resourceName );
S2CORE_API unsigned char *bytes( const std::string &resourceName );
}

}


#endif