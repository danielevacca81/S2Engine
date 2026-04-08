#ifndef MD5_H
#define MD5_H

#include "s2Engine_API.h"

#include <string>

namespace s2 {
namespace MD5 {

std::string S2ENGINE_API compute( const std::string& str );

}
}

#endif