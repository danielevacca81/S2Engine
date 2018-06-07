// ImageBuffer.hpp
//
#include "ImageBuffer.h"

#include <fstream>
#include <cassert>
#include <cstring>  //Needed for Linux platform

namespace s2 {

// -------------------------------------------------------------------------------------------------
template<typename T>
ImageBuffer<T>::ImageBuffer()
	: _w( 0 )
	, _h( 0 )
	, _numCh( 0 )
	, _data( 0 )
{}

// -------------------------------------------------------------------------------------------------
template<typename T>
ImageBuffer<T>::ImageBuffer( int width, int height, int channels, T *pxl )
	: _w( 0 )
	, _h( 0 )
	, _numCh( 0 )
	, _data( 0 )
{
	create( width, height, channels, pxl );
}

// -------------------------------------------------------------------------------------------------
template<typename T>
ImageBuffer<T>::~ImageBuffer()
{
	destroy();
}

// -------------------------------------------------------------------------------------------------
template<typename T>
void ImageBuffer<T>::create( int width, int height, int channels, T *pxl )
{
	destroy();

	_w     = width;
	_h     = height;
	_numCh = channels;
	_data  = new T[_w * _h * _numCh];

	clear();

	if( pxl )
		setValues( pxl );
}

// -------------------------------------------------------------------------------------------------
template<typename T>
void ImageBuffer<T>::setValues( T *values )
{
	memcpy( _data, values, sizeof( T )*_w*_h*_numCh );
}

// -------------------------------------------------------------------------------------------------
template<typename T>
void ImageBuffer<T>::clear()
{
	memset( _data, 0, sizeof( T )*_w*_h*_numCh );
}

// -------------------------------------------------------------------------------------------------
template<typename T>
void ImageBuffer<T>::destroy()
{
	//if(_data) 
	delete[] _data;
	_data  =  nullptr;

	_w     = 0;
	_h     = 0;
	_numCh = 0;
}

// ----------------------------------------------------------------------------------------------
template<typename T>
T &ImageBuffer<T>::operator[]( int i )
{
	assert( _data );
	assert( i >= 0 && i < _w*_h*_numCh );

	return _data[i];
}

// ----------------------------------------------------------------------------------------------
template<typename T>
void ImageBuffer<T>::resize( int width, int height, int channels )
{
	create( width, height, channels, nullptr );
}

// ----------------------------------------------------------------------------------------------
static inline bool dumpTGA( const std::string &targaFilename, unsigned char *pixels, short numChannels, short w, short h )
{
	const char cTgaHeader[12] = { 0, 0, 2, 0,  0, 0, 0, 0,  0, 0, 0, 0 };

	std::ofstream file;

	// Open the file for output
	file.open( targaFilename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc );

	if( !file )
		return false;

	// swap RGB -> BGR
	char *pxl = new char[w*h*numChannels];
	memcpy( pxl, pixels, sizeof( char )*w*h*numChannels );
	for( int i=0; i < w*h*numChannels; i+=numChannels )
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

	file.write( (char*) pxl, w * h * numChannels * sizeof( char ) );

	// Close file again
	file.close();
	delete[] pxl;

	return true;
}

//-------------------------------------------------------------------------------------------------
template<typename T>
bool ImageBuffer<T>::dump( const std::string &tgaFilename ) const
{
	return dumpTGA( tgaFilename, _data, _numCh, _w, _h );
}

}