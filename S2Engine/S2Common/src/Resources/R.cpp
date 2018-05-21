// R.cpp
// 
#include "R.h"

#ifdef _WIN32
#include "Win32/EmbeddedResources.h"
#endif // _WIN32

#include "Common/Strings.h"

namespace R
{

// ------------------------------------------------------------------------------------------------
void init()
{
#ifdef _WIN32
	Win32::EmbeddedResources::init();
#endif // _WIN32
}

// ------------------------------------------------------------------------------------------------
void free()
{
#ifdef _WIN32
	Win32::EmbeddedResources::free();
#endif // _WIN32
}

// ------------------------------------------------------------------------------------------------
std::string string( const std::string &resourceName )
{
#ifdef _WIN32
	auto r = String::split<std::string>( resourceName, "/" );

	if( !Win32::EmbeddedResources::exists(r[0],r[1]) )
		return "";

	return std::string( (char*)( Win32::EmbeddedResources::get( r[0],r[1] ) ) );
#endif // _WIN32
}

// ------------------------------------------------------------------------------------------------
unsigned char *bytes ( const std::string &resourceName )
{
#ifdef _WIN32
	auto r = String::split<std::string>( resourceName, "/" );

	if( !Win32::EmbeddedResources::exists(r[0],r[1]) )
		return 0;

	return (unsigned char*)( Win32::EmbeddedResources::get( r[0],r[1] ) );
#endif // _WIN32
}


}