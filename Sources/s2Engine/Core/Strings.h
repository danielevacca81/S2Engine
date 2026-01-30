// Strings.h
//
#ifndef CORE_STRINGS_H
#define CORE_STRINGS_H

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <optional>
#include <locale>
#include <ranges>
#include <cwctype>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

namespace s2 {
namespace String
{

// ------------------------------------------------------------------------------------------------
template<typename CharT>
struct string_view_type;

template<>
struct string_view_type<char> { using type = std::string_view; };

template<>
struct string_view_type<wchar_t> { using type = std::wstring_view; };

template<typename CharT>
using string_view_t = typename string_view_type<CharT>::type;

// ================================================================================================
// TOKENIZATION & SPLITTING
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template <typename T>
inline std::vector<T> tokenize( const T& str, const T& startSep = T( " " ), const T& endSep = T( " " ) )
{
	std::vector<T> out;
	std::size_t curr = 0;

	while( curr < str.length() )
	{
		// Find the start separator
		std::size_t tkStart = str.find( startSep, curr );
		if( tkStart == T::npos )
			break;

		// Skip the start separator
		tkStart += startSep.length();

		// Find the end separator
		std::size_t tkEnd = str.find( endSep, tkStart );
		if( tkEnd == T::npos )
			tkEnd = str.length();

		// Extract the token if valid
		if( tkEnd > tkStart )
			out.push_back( str.substr( tkStart, tkEnd - tkStart ) );

		curr = tkEnd + endSep.length();
	}

	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline std::vector<T> split( const T& str, const T& delimiters, bool keepEmpty = false )
{
	std::vector<T> out;
	std::size_t start = 0;
	std::size_t pos = str.find_first_of( delimiters, start );

	while( pos != T::npos )
	{
		if( pos != start || keepEmpty )
			out.push_back( str.substr( start, pos - start ) );

		start = pos + 1;
		pos = str.find_first_of( delimiters, start );
	}

	if( start < str.length() || keepEmpty )
		out.push_back( str.substr( start ) );

	return out;
}

// ------------------------------------------------------------------------------------------------
// String view split overloads for both char and wchar_t
inline auto split( std::string_view str, char delimiter )
{
	return str
		| std::views::split( delimiter )
		| std::views::transform( [] ( auto&& rng ) {
		return std::string_view( &*rng.begin(), std::ranges::distance( rng ) );
	} );
}

inline auto split( std::wstring_view str, wchar_t delimiter )
{
	return str
		| std::views::split( delimiter )
		| std::views::transform( [] ( auto&& rng ) {
		return std::wstring_view( &*rng.begin(), std::ranges::distance( rng ) );
	} );
}

// ================================================================================================
// STRING CHECKS
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline bool startsWith( std::basic_string_view<CharT> str, std::basic_string_view<CharT> key )
{
	return str.starts_with( key );
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline bool endsWith( std::basic_string_view<CharT> str, std::basic_string_view<CharT> key )
{
	return str.ends_with( key );
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline bool contains( std::basic_string_view<CharT> str, std::basic_string_view<CharT> key )
{
	return str.find( key ) != std::basic_string_view<CharT>::npos;
}

// ================================================================================================
// STRING EXTRACTION
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::optional<std::basic_string<CharT>> after( std::basic_string_view<CharT> str,
													  std::basic_string_view<CharT> delimiter )
{
	const size_t pos = str.find( delimiter );

	if( pos == std::basic_string_view<CharT>::npos )
		return std::nullopt;

	const size_t start = pos + delimiter.length();
	return std::basic_string<CharT>( str.substr( start ) );
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::optional<std::basic_string<CharT>> before( std::basic_string_view<CharT> str,
													   std::basic_string_view<CharT> delimiter )
{
	const size_t pos = str.find( delimiter );

	if( pos == std::basic_string_view<CharT>::npos )
		return std::nullopt;

	return std::basic_string<CharT>( str.substr( 0, pos ) );
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::optional<std::basic_string<CharT>> between( std::basic_string_view<CharT> str,
														std::basic_string_view<CharT> start,
														std::basic_string_view<CharT> end )
{
	const size_t startPos = str.find( start );
	if( startPos == std::basic_string_view<CharT>::npos )
		return std::nullopt;

	const size_t contentStart = startPos + start.length();
	const size_t endPos = str.find( end, contentStart );

	if( endPos == std::basic_string_view<CharT>::npos )
		return std::nullopt;

	return std::basic_string<CharT>( str.substr( contentStart, endPos - contentStart ) );
}

// ================================================================================================
// WIDE/MULTIBYTE CONVERSIONS
// ================================================================================================

// ------------------------------------------------------------------------------------------------
// MODERNIZED: wstring -> string conversion without deprecated codecvt
// Uses WideCharToMultiByte on Windows, mbsrtowcs on Linux
inline std::string toStdString( const std::wstring& wstr )
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
inline std::wstring toStdWString( const std::string& str )
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

// ================================================================================================
// CASE CONVERSIONS
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T toUpper( const T& str )
{
	T out( str );

	if constexpr( std::is_same_v<T, std::string> )
	{
		std::transform( out.begin(), out.end(), out.begin(),
						[] ( unsigned char c ) { return ::toupper( c ); } );
	}
	else if constexpr( std::is_same_v<T, std::wstring> )
	{
		std::transform( out.begin(), out.end(), out.begin(),
						[] ( wchar_t c ) { return ::towupper( c ); } );
	}

	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T toLower( const T& str )
{
	T out( str );

	if constexpr( std::is_same_v<T, std::string> )
	{
		std::transform( out.begin(), out.end(), out.begin(),
						[] ( unsigned char c ) { return ::tolower( c ); } );
	}
	else if constexpr( std::is_same_v<T, std::wstring> )
	{
		std::transform( out.begin(), out.end(), out.begin(),
						[] ( wchar_t c ) { return ::towlower( c ); } );
	}

	return out;
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::basic_string<CharT> toUpperUnicode( const std::basic_string<CharT>& str,
												const std::locale& loc = std::locale( "" ) )
{
	std::basic_string<CharT> out( str );
	std::transform( out.begin(), out.end(), out.begin(),
					[&loc] ( CharT c ) { return std::toupper( c, loc ); } );
	return out;
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::basic_string<CharT> toLowerUnicode( const std::basic_string<CharT>& str,
												const std::locale& loc = std::locale( "" ) )
{
	std::basic_string<CharT> out( str );
	std::transform( out.begin(), out.end(), out.begin(),
					[&loc] ( CharT c ) { return std::tolower( c, loc ); } );
	return out;
}

// ================================================================================================
// TRIMMING & WHITESPACE
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::basic_string<CharT> trimLeft( std::basic_string_view<CharT> str )
{
	auto start = std::find_if_not( str.begin(), str.end(),
								   [] ( CharT c ) {
		if constexpr( std::is_same_v<CharT, char> )
			return std::isspace( static_cast<unsigned char>( c ) );
		else
			return ::iswspace( c );
	} );
	return std::basic_string<CharT>( start, str.end() );
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::basic_string<CharT> trimRight( std::basic_string_view<CharT> str )
{
	auto end = std::find_if_not( str.rbegin(), str.rend(),
								 [] ( CharT c ) {
		if constexpr( std::is_same_v<CharT, char> )
			return std::isspace( static_cast<unsigned char>( c ) );
		else
			return ::iswspace( c );
	} );
	return std::basic_string<CharT>( str.begin(), end.base() );
}

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::basic_string<CharT> trim( std::basic_string_view<CharT> str )
{
	return trimLeft<CharT>( trimRight<CharT>( str ) );
}

// ================================================================================================
// STRING MANIPULATION
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline std::basic_string<CharT> replaceAll( std::basic_string<CharT> str,
											std::basic_string_view<CharT> from,
											std::basic_string_view<CharT> to )
{
	if( from.empty() )
		return str;

	size_t pos = 0;
	while( ( pos = str.find( from, pos ) ) != std::basic_string<CharT>::npos )
	{
		str.replace( pos, from.length(), to );
		pos += to.length();
	}
	return str;
}

// ================================================================================================
// VALIDATION
// ================================================================================================

// ------------------------------------------------------------------------------------------------
template<typename CharT>
inline bool isNumeric( std::basic_string_view<CharT> str )
{
	if( str.empty() )
		return false;

	// Automatic character type detection
	CharT minusChar, plusChar, dotChar;
	if constexpr( std::is_same_v<CharT, char> )
	{
		minusChar = '-';
		plusChar = '+';
		dotChar = '.';
	}
	else
	{
		minusChar = L'-';
		plusChar = L'+';
		dotChar = L'.';
	}

	auto it = str.begin();
	if( *it == minusChar || *it == plusChar )
		++it;

	bool hasDigit = false;
	bool hasDot = false;

	for( ; it != str.end(); ++it )
	{
		bool isDigit;
		if constexpr( std::is_same_v<CharT, char> )
			isDigit = std::isdigit( static_cast<unsigned char>( *it ) );
		else
			isDigit = std::iswdigit( *it );

		if( isDigit )
			hasDigit = true;
		else if( *it == dotChar && !hasDot )
			hasDot = true;
		else
			return false;
	}

	return hasDigit;
}

// ================================================================================================
// JOIN
// ================================================================================================

// ------------------------------------------------------------------------------------------------
// Overload for std::string with string_view delimiter
template <std::ranges::range Range>
	requires std::convertible_to<std::ranges::range_value_t<Range>, std::string>
inline std::string join( const Range& strings, std::string_view delimiter )
{
	if( std::ranges::empty( strings ) )
		return {};

	std::string result;
	bool first = true;

	for( const auto& str : strings )
	{
		if( !first )
			result += delimiter;
		result += str;
		first = false;
	}

	return result;
}

// ------------------------------------------------------------------------------------------------
// Overload for std::wstring with wstring_view delimiter
template <std::ranges::range Range>
	requires std::convertible_to<std::ranges::range_value_t<Range>, std::wstring>
inline std::wstring join( const Range& strings, std::wstring_view delimiter )
{
	if( std::ranges::empty( strings ) )
		return {};

	std::wstring result;
	bool first = true;

	for( const auto& str : strings )
	{
		if( !first )
			result += delimiter;
		result += str;
		first = false;
	}

	return result;
}

}
}
#endif // !CORE_STRINGS_H