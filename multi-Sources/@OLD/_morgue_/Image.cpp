// Image.cpp
//
#include "Image.h"

#include <fstream>

using namespace OpenGL;

//-------------------------------------------------------------------------------------------------
Image::Image()
: w(0),
  h(0),
  numCh(0),
  data(0)
{}

//-------------------------------------------------------------------------------------------------
Image::Image( int width, int height, int channels, void *pxl )
{
	create(width,height,channels,pxl);
}

//-------------------------------------------------------------------------------------------------
Image::~Image()
{
	clear();
}

//-------------------------------------------------------------------------------------------------
void Image::create( int width, int height, int channels, void *pxl)
{
	w     = width;
	h     = height;
	numCh = channels;
	data  = pxl;
}

//-------------------------------------------------------------------------------------------------
void Image::clear()
{
	if(data) delete [] data;

	w     = 0;
	h     = 0;
	numCh = 0;
	data  = 0;
}


// ----------------------------------------------------------------------------------------------
bool dumpTGA(std::string targaFilename, unsigned char *pixels, short numChannels, short w, short h)
{
	const char cTgaHeader[12] = { 0, 0, 2, 0,  0, 0, 0, 0,  0, 0, 0, 0 };

	std::ofstream file;

	// Open the file for output
	file.open(targaFilename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc); 

	if (!file)
		return false;

	// swap RGB -> BGR
	char *pxl = new char[w*h*numChannels];
	memcpy(pxl,pixels,sizeof(char)*w*h*numChannels);
	for(int i=0; i<w*h*numChannels; i+=numChannels)
	{
		pxl[i + 0] = pxl[i + 0] ^ pxl[i + 2];
		pxl[i + 2] = pxl[i + 0] ^ pxl[i + 2];
		pxl[i + 0] = pxl[i + 0] ^ pxl[i + 2];
	}

	// Create some temporaries
	char bitdepth = numChannels * 8;
	char bituse = 0;

	// Write out all targa image data
	file.write(cTgaHeader, 12 * sizeof(char));

	file.write((char*)&w, sizeof(short));
	file.write((char*)&h, sizeof(short));
	file.write(&bitdepth, sizeof(char));
	file.write(&bituse, sizeof(char));

	file.write((char*)pxl, w * h * numChannels * sizeof(char));

	// Close file again
	file.close();
	delete [] pxl;

	return true;
}

//-------------------------------------------------------------------------------------------------
bool Image::dump(const std::string &tgaFilename)
{
	return dumpTGA(tgaFilename,(unsigned char*)data,numCh,w,h);
}
