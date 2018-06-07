// Win32.h
// 
#ifndef WIN32_ONCE
#define WIN32_ONCE

#include "s2Core_API.h"

#include <string>
#include <vector>

namespace Win32 {

std::string S2CORE_API executableFileName();
std::string S2CORE_API productName( const std::string &fileName );
std::string S2CORE_API fileVersion( const std::string &fileName );
void        S2CORE_API enableCrashReport();
std::string S2CORE_API systemName();
bool        S2CORE_API sendMail( const std::vector<std::string> &recipient, const std::string &subject, const std::string &body, const std::string &attachment = std::string("") );
size_t      S2CORE_API peakMemoryUsage();
size_t      S2CORE_API currentMemoryUsage();

}

#endif