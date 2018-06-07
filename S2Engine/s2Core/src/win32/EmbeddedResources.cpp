// EmbeddedResources.cpp
// 
#include "EmbeddedResources.h"

#include <windows.h>
#include <vector>
#include <sstream>

#include "utils/String.h"
#include "utils/MemCheck.h"


// ------------------------------------------------------------------------------------------------
namespace s2 {
namespace Win32 {

std::map<std::string, EmbeddedResources::ResourceEntry> EmbeddedResources::_resources;

static BOOL EnumTypesFunc( HMODULE hModule, LPTSTR lpType, LONG lParam );
static BOOL EnumNamesFunc( HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam );

static std::vector<USHORT>      g_types;
static std::vector< std::pair<std::wstring, std::wstring> > g_names;

// ------------------------------------------------------------------------------------------------
const unsigned char *EmbeddedResources::get( const std::string &type, const std::string &name )
{
	assert( !_resources.empty() );

	auto res = _resources.find( String::toUpper( name ) );

	if( res == _resources.end() )                  return 0;
	if( res->second.type != String::toUpper( type ) ) return 0;

	return res->second.data;
}

// ------------------------------------------------------------------------------------------------
const bool EmbeddedResources::exists( const std::string &type, const std::string &name )
{
	assert( !_resources.empty() );

	auto res = _resources.find( String::toUpper( name ) );

	return res != _resources.end();
}


// ------------------------------------------------------------------------------------------------
void EmbeddedResources::free()
{
	for( auto it = _resources.begin();
		 it != _resources.end();
		 ++it )
		delete[] it->second.data;
}

// ------------------------------------------------------------------------------------------------
bool EmbeddedResources::init()
{
	HMODULE handle = ::GetModuleHandle( 0 );

	::EnumResourceTypes( handle, (ENUMRESTYPEPROC) EnumTypesFunc, 0 );

	for( size_t i=0; i < g_names.size(); ++i )
	{
		HRSRC   rc = ::FindResource( handle, g_names[i].second.c_str(), g_names[i].first.c_str() );

		if( !rc )
			continue;

		// get a pointer to resource
		HGLOBAL rcData = ::LoadResource( handle, rc );
		DWORD   size   = ::SizeofResource( handle, rc );
		unsigned char *data   = static_cast<unsigned char *>( ::LockResource( rcData ) );

		// copy resource and terminate
		unsigned char *buffer = new unsigned char[size + 1];
		memcpy( buffer, data, size );
		buffer[size] = '\0';

		ResourceEntry entry;
		entry.type = String::toStdString( g_names[i].first );
		entry.name = String::toStdString( g_names[i].second );
		entry.size = size;
		entry.data = (unsigned char *) buffer;

		_resources[entry.name] = entry;
	}

	return !_resources.empty();
}

// ------------------------------------------------------------------------------------------------
static BOOL EnumTypesFunc(
	HMODULE hModule,  // module handle
	LPTSTR lpType,    // address of resource type
	LONG lParam )      // extra parameter, could be used for error checking
{
	// The type may be a string or an unsigned decimal
	// integer, so test before printing.
	if( !IS_INTRESOURCE( lpType ) )
	{
		/**/
	}
	else
	{
		g_types.push_back( (USHORT) lpType );
	}

	// Find the names of all resources of type lpType.
	::EnumResourceNames( hModule,
						 lpType,
						 (ENUMRESNAMEPROC) EnumNamesFunc,
						 0 );

	return TRUE;
}

// ------------------------------------------------------------------------------------------------  
static BOOL EnumNamesFunc(
	HMODULE hModule,  // module handle
	LPCTSTR lpType,   // address of resource type
	LPTSTR lpName,    // address of resource name
	LONG lParam )      // extra parameter, could be used for error checking
{
	// The name may be a string or an unsigned decimal
	// integer, so test before printing.
	if( !IS_INTRESOURCE( lpName ) )
	{
		std::wstring name( lpName );
		std::wstring type( lpType );

		g_names.push_back( std::make_pair( type, name ) );
	}
	else
	{
	}

	return TRUE;
}

}

}