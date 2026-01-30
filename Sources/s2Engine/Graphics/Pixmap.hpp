// Pixmap.hpp
//
#include "Pixmap.h"

#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>  //Needed for Linux platform
#include <array>    // std::array
#include <limits>   // std::numeric_limits
#include <algorithm> // std::swap

using namespace s2;

// ------------------------------------------------------------------------------------------------
template<typename T>
Pixmap<T>::Pixmap( int32_t width, int32_t height, int32_t channels, T* pxl )
	: _w( width )
	, _h( height )
	, _numCh( channels )
{
	// parameter checks
	assert( width >= 0 && "Width must be non-negative" );
	assert( height >= 0 && "Height must be non-negative" );
	assert( channels >= 0 && "Channels must be non-negative" );

	// Overflow check before allocation
	const size_t totalSize = static_cast<size_t>( width )
		* static_cast<size_t>( height )
		* static_cast<size_t>( channels );

	constexpr size_t maxSize = std::numeric_limits<size_t>::max() / sizeof( T );
	assert( totalSize <= maxSize && "Pixmap size would cause overflow" );

	_w = static_cast<uint32_t>( width );
	_h = static_cast<uint32_t>( height );
	_numCh = static_cast<uint32_t>( channels );

	_pixels.resize( totalSize, T {} );

	setPixels( pxl );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Pixmap<T>::setPixels( T* values )
{
	if( !values || _pixels.empty() )
		return;
	
	std::memcpy( _pixels.data(), values, sizeof( T ) * _pixels.size() );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Pixmap<T>::clear()
{
	if( _pixels.empty() )
		return;

	std::memset( _pixels.data(), 0, sizeof( T ) * _pixels.size() );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
const T& Pixmap<T>::operator[]( int32_t i ) const
{
	assert( i >= 0 && static_cast<size_t>( i ) <  _pixels.size() );
	return _pixels[i];
}

// ------------------------------------------------------------------------------------------------
template<typename T>
T& Pixmap<T>::operator[]( int32_t i )
{
	assert( i >= 0 && static_cast<size_t>( i ) <  _pixels.size() );
	return _pixels[i];
}

// ------------------------------------------------------------------------------------------------
template<typename T>
void Pixmap<T>::resize( int32_t width, int32_t height, int32_t channels )
{
	assert( width >= 0 && "Width must be non-negative" );
	assert( height >= 0 && "Height must be non-negative" );
	assert( channels >= 0 && "Channels must be non-negative" );

	// Controllo overflow
	const size_t totalSize = static_cast<size_t>( width ) *
		static_cast<size_t>( height ) *
		static_cast<size_t>( channels );

	constexpr size_t maxSize = std::numeric_limits<size_t>::max() / sizeof( T );
	assert( totalSize <= maxSize && "Pixmap size would cause overflow" );

	_w     = static_cast<uint32_t>( width );
	_h     = static_cast<uint32_t>( height );
	_numCh = static_cast<uint32_t>( channels );

	_pixels.resize( totalSize, T {} );
}

// ------------------------------------------------------------------------------------------------
template<typename T>
bool Pixmap<T>::dump( const std::string& tgaFilename ) const
{
	static_assert( sizeof( T ) == 1, "TGA dump not supported for multi-byte pixel types" );

	// Validazione parametri
	if( _pixels.empty() || _w == 0 || _h == 0 )
		return false;

	if( _w > 65535 || _h > 65535 || _numCh < 1 || _numCh > 4 )
		return false; // TGA limitations

	auto dumpTGA = [] ( const std::string& targaFilename, const unsigned char* const pixels,
						uint16_t numChannels, uint16_t w, uint16_t h )
	{
		const std::array<uint8_t, 12> cTgaHeader = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		std::ofstream file( targaFilename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc );
		if( !file )
			return false;

		const size_t totalBytes = static_cast<size_t>( w ) * h * numChannels;
		std::vector<uint8_t> pxl( pixels, pixels + totalBytes );

		// Swap RGB -> BGR
		if( numChannels >= 3 )
		{
			for( size_t i = 0; i < totalBytes; i += numChannels )
				std::swap( pxl[i + 0], pxl[i + 2] );
		}

		// Create some temporaries
		const uint8_t bitdepth = numChannels * 8;
		const uint8_t bituse = 0;

		// Write TGA header
		file.write( reinterpret_cast<const char*>( cTgaHeader.data() ), cTgaHeader.size() );
		file.write( reinterpret_cast<const char*>( &w ),        sizeof( w ) );
		file.write( reinterpret_cast<const char*>( &h ),        sizeof( h ) );
		file.write( reinterpret_cast<const char*>( &bitdepth ), sizeof( bitdepth ) );
		file.write( reinterpret_cast<const char*>( &bituse ),   sizeof( bituse ) );

		// Write pixel data
		file.write( reinterpret_cast<const char*>( pxl.data() ), totalBytes );

		return file.good();
	};

	return dumpTGA( tgaFilename,
					reinterpret_cast<const unsigned char*>( _pixels.data() ),
					static_cast<uint16_t>( _numCh ),
					static_cast<uint16_t>( _w ),
					static_cast<uint16_t>( _h ) );
}