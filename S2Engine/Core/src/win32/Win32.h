// Win32.h
// 
#ifndef WIN32_ONCE
#define WIN32_ONCE

#include "Core_API.h"

#include <string>
#include <vector>

namespace Win32 {

std::string CORE_API executableFileName();
std::string CORE_API productName( const std::string &fileName );
std::string CORE_API fileVersion( const std::string &fileName );
void        CORE_API enableCrashReport();
std::string CORE_API systemName();
bool        CORE_API sendMail( const std::vector<std::string> &recipient, const std::string &subject, const std::string &body, const std::string &attachment = std::string("") );
size_t      CORE_API peakMemoryUsage();
size_t      CORE_API currentMemoryUsage();

}

#endif