// Pixmap.hpp
//
#include "Pixmap.h"

#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>  //Needed for Linux platform

// -------------------------------------------------------------------------------------------------
template<typename T>
Pixmap<T>::Pixmap( int32_t width, int32_t height, int32_t channels, T* pxl )
	: _w( width )
	, _h( height )
	, _numCh( channels )
{
	assert( width >= 0 );
	assert( height >= 0 );
	assert( channels >= 0 );

	_pixels = std::vector<T>( _w * _h * _numCh, 0 );

	setPixels( pxl );
}

// -------------------------------------------------------------------------------------------------
template<typename T>
void Pixmap<T>::setPixels( T* values )
{
	if( values )
		memcpy( &_pixels[0], values, sizeof( T ) * _w * _h * _numCh );
}

// -------------------------------------------------------------------------------------------------
template<typename T>
void Pixmap<T>::clear()
{
	memset( &_pixels[0], 0, sizeof( T ) * _w * _h * _numCh );
}

// ----------------------------------------------------------------------------------------------
template<typename T>
T& Pixmap<T>::operator[]( int i ) const
{
	assert( i >= 0 && i < _w * _h * _numCh );
	return _pixels.at( i );
}

// ----------------------------------------------------------------------------------------------
template<typename T>
void Pixmap<T>::resize( int32_t width, int32_t height, int32_t channels )
{
	_pixels = std::vector<T>( _w * _h * _numCh, 0 );
}

//-------------------------------------------------------------------------------------------------
template<typename T>
bool Pixmap<T>::dump( const std::string& tgaFilename ) const
{
	auto dumpTGA = [] ( const std::string& targaFilename, const unsigned char* const pixels, uint16_t numChannels, uint16_t w, uint16_t h )
	{
		const char cTgaHeader[12] = { 0, 0, 2, 0,  0, 0, 0, 0,  0, 0, 0, 0 };

		std::ofstream file( targaFilename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc );
		if( !file )
			return false;

		// swap RGB -> BGR
		std::vector<char> pxl( w * h * numChannels );
		memcpy( &pxl[0], pixels, sizeof( char ) * w * h * numChannels );
		for( int i = 0; i < w * h * numChannels; i += numChannels )
		{
			pxl[i + 0] = pxl[i + 0] ^ pxl[i + 2];
			pxl[i + 2] = pxl[i + 0] ^ pxl[i + 2];
			pxl[i + 0] = pxl[i + 0] ^ pxl[i + 2];
		}

		// Create some temporaries
		char bitdepth = numChannels * 8;
		char bituse = 0;

		// Write out all targa image data
		file.write( cTgaHeader, 12 * sizeof( char ) );

		file.write( (char*) &w, sizeof( short ) );
		file.write( (char*) &h, sizeof( short ) );
		file.write( &bitdepth, sizeof( char ) );
		file.write( &bituse, sizeof( char ) );

		file.write( (char*) &pxl[0], w * h * numChannels * sizeof( char ) );
		return true;
	};

	return dumpTGA( tgaFilename, reinterpret_cast<const unsigned char* const>( &_pixels[0] ), _numCh, _w, _h );
}