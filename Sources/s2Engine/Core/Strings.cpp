// Strings.cpp
//
#include "Strings.h"

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


namespace s2 {
namespace String
{

// ================================================================================================
// WIDE/MULTIBYTE CONVERSIONS
// ================================================================================================

// ------------------------------------------------------------------------------------------------
std::string toStdString( const std::wstring& wstr )
{
	if( wstr.empty() )
		return {};

#if defined(_WIN32) || defined(_WIN64)
	// Windows: use native API
	int size = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr );
	if( size <= 0 ) return {};

	std::string result( size - 1, '\0' );
	WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), -1, result.data(), size, nullptr, nullptr );
	return result;
#else
	// Linux/Unix: use wcsrtombs
	std::mbstate_t state = std::mbstate_t();
	const wchar_t* src = wstr.c_str();

	size_t len = std::wcsrtombs( nullptr, &src, 0, &state );
	if( len == static_cast<size_t>( -1 ) )
		return {};

	std::string result( len, '\0' );
	std::wcsrtombs( result.data(), &src, len, &state );
	return result;
#endif
}

// ------------------------------------------------------------------------------------------------
std::wstring toStdWString( const std::string& str )
{
	if( str.empty() )
		return {};

#if defined(_WIN32) || defined(_WIN64)
	// Windows: use native API
	int size = MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, nullptr, 0 );
	if( size <= 0 ) return {};

	std::wstring result( size - 1, L'\0' );
	MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, result.data(), size );
	return result;
#else
	// Linux/Unix: use mbsrtowcs
	std::mbstate_t state = std::mbstate_t();
	const char* src = str.c_str();

	size_t len = std::mbsrtowcs( nullptr, &src, 0, &state );
	if( len == static_cast<size_t>( -1 ) )
		return {};

	std::wstring result( len, L'\0' );
	std::mbsrtowcs( result.data(), &src, len, &state );
	return result;
#endif
}

}
}

