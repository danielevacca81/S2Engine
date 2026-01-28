//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//
#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include "s2Engine_API.h"

#include <string>

namespace Base64 {

std::string S2ENGINE_API encode( const std::string &s );
std::string S2ENGINE_API decode( const std::string &s );

}

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
