// Strings.h
//
#ifndef CORE_STRINGS_H
#define CORE_STRINGS_H

#include <string>
#include <vector>
#include <algorithm>
#include <codecvt>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if (defined(_WIN32) || defined(_WIN64))
#pragma warning (disable: 4996)
#endif

namespace String
{

// ------------------------------------------------------------------------------------------------
template <typename T>
inline std::vector<T> tokenize( const T &str, const T &startSeparator = T( " " ), const T &endSeparator = T( " " ) )
{
	std::vector<T> out;

	std::size_t curr = 0;
	do
	{
		std::size_t tkStart = str.find( startSeparator, curr );
		std::size_t tkEnd = str.find( endSeparator, tkStart );

		if( tkEnd > tkStart )
			out.push_back( str.substr( tkStart, tkEnd - tkStart ) );

		curr = tkEnd;
	} while( curr != T::npos );

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
			out.push_back( str.substr( start, pos - start ) );

		start = pos + 1;
		pos=str.find_first_of( delimiters, start );
	}

	if( start < str.length() )
		out.push_back( str.substr( start ) );

	return out;
}

// ------------------------------------------------------------------------------------------------
inline bool startsWith( const std::string &str, const std::string &key )
{
	const std::string left = str.substr( 0, key.size() );

	return left == key;
}

// ------------------------------------------------------------------------------------------------
inline bool endsWith( const std::string &str, const std::string &key )
{
	if( str.size() < key.size() )
		return false;

	const std::string right = str.substr( str.size() - key.size(), key.size() );

	return right == key;
}

// ------------------------------------------------------------------------------------------------
inline std::string after( const std::string &str, const std::string &delimiter )
{
	const size_t pos = str.find_first_of( delimiter ) + 1;

	std::string ret = str.substr( pos, std::string::npos );

	return ret;
}

//Windows uses a 16-bit wchar_t, while Linux 32-bit
template <int N> struct get_codecvt_utf8_wchar_impl;
template <> struct get_codecvt_utf8_wchar_impl<16>
{
	using type = std::codecvt_utf8_utf16<wchar_t>;
};
template <> struct get_codecvt_utf8_wchar_impl<32>
{
	using type = std::codecvt_utf8<wchar_t>;
};

// ------------------------------------------------------------------------------------------------
inline std::string toStdString( const std::wstring &str )
{
	if( str.empty() )
		return std::string( "" );

	using cvt_utf8_wchar = get_codecvt_utf8_wchar_impl<sizeof( wchar_t ) * 8>::type;
	std::string s;
	std::wstring_convert<cvt_utf8_wchar> converter;
	try
	{
		s = converter.to_bytes( str );
	}
	catch( std::range_error e )
	{
		s.clear();
	}
	return s;
}

// ------------------------------------------------------------------------------------------------
inline std::wstring toStdWString( const std::string &str )
{
	if( str.empty() )
		return std::wstring( L"" );

	using cvt_utf8_wchar = get_codecvt_utf8_wchar_impl<sizeof( wchar_t ) * 8>::type;
	std::wstring s;
	std::wstring_convert<cvt_utf8_wchar> converter;
	try
	{
		s = converter.from_bytes( str );
	}
	catch( std::range_error e )
	{
		s = L"";
	}
	return s;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T toUpper( const T &str )
{
	T out( str );

	std::transform( out.begin(), out.end(), out.begin(), ::toupper );
	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T toLower( const T &str )
{
	T out( str );

	std::transform( out.begin(), out.end(), out.begin(), ::tolower );
	return out;
}

}

#endif