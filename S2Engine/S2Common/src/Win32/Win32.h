// Win32.h
// 
#ifndef WIN32_ONCE
#define WIN32_ONCE

#include "S2CommonAPI.h"

#include <string>
#include <vector>

namespace Win32 {

std::string S2COMMON_API executableFileName();
std::string S2COMMON_API productName( const std::string &fileName );
std::string S2COMMON_API fileVersion( const std::string &fileName );
void        S2COMMON_API enableCrashReport();
std::string S2COMMON_API systemName();
bool        S2COMMON_API sendMail( const std::vector<std::string> &recipient, const std::string &subject, const std::string &body, const std::string &attachment = std::string("") );
size_t      S2COMMON_API peakMemoryUsage();
size_t      S2COMMON_API currentMemoryUsage();

}

#endif