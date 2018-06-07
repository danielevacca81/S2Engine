// EmbeddedResources.h
// 
#ifndef EMBEDDED_RESOURCES_ONCE
#define EMBEDDED_RESOURCES_ONCE

#include "s2Core_API.h"

#include <cassert>
#include <string>
#include <map>

namespace Win32 {

class S2CORE_API EmbeddedResources
{
private:
	struct ResourceEntry
	{
		std::string          name;
		std::string          type;
		int                  size;
		const unsigned char *data;
	};

	static std::map<std::string,ResourceEntry> _resources;


public:
	static bool init();
	static void free();
	
	static const bool            exists( const std::string &type, const std::string &name );
	static const unsigned char * get( const std::string &type, const std::string &name );
};

}
#endif