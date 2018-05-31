//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//
#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include "Core_API.h"

#include <string>

std::string CORE_API base64_encode( const std::string & );
std::string CORE_API base64_encode( unsigned char const*, unsigned int len );
std::string CORE_API base64_decode( std::string const& s );

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
