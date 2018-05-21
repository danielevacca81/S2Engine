// Strings.h
//
#ifndef STRINGS_ONCE
#define STRINGS_ONCE

#include <string>
#include <vector>
#include <algorithm>
#include <codecvt>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

namespace String {

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T toUpper( const T &str )
{
	T out(str);

	std::transform(out.begin(), out.end(), out.begin(), ::toupper);
	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T toLower( const T &str )
{
	T out(str);

	std::transform(out.begin(), out.end(), out.begin(), ::tolower);
	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline std::vector<T> split( const T &str, const T &delimiters )
{
	std::vector<T> out;

	std::size_t start = 0;
	std::size_t pos   = str.find_first_of( delimiters, start );
	
	while( pos != T::npos )
	{
		if( pos != start )
			out.push_back( str.substr(start,pos-start) );
		
		start = pos+1;
		pos=str.find_first_of(delimiters,start);
	}
	
	if( start<str.length() )
		out.push_back( str.substr(start) );

	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline bool startsWith( const T &str, const T &key )
{
	const T left = str.substr( 0,key.size() );

	return left == key;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline bool endsWith( const T &str, const T &key )
{
	if( str.size() < key.size() )
		return false;

	const T right = str.substr( str.size()-key.size(),key.size() );

	return right == key;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T after( const T &str, const T &delimiter )
{
	const size_t pos = str.find_first_of( delimiter )+1;

	T ret = str.substr( pos, std::T::npos );

	return ret;
}

// ------------------------------------------------------------------------------------------------
inline std::string toStdString( const std::wstring &str )
{
	std::wstring_convert< std::codecvt_utf8_utf16<wchar_t> > converter;
	return converter.to_bytes(str);
}

// ------------------------------------------------------------------------------------------------
inline std::wstring toStdWString( const std::string &str )
{
	std::wstring_convert< std::codecvt_utf8_utf16<wchar_t> > converter;
	return converter.from_bytes(str);
}

}
#endif