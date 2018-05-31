// Utils.h
//
#ifndef UTILS_ONCE
#define UTILS_ONCE

#include <string>
#include <fstream>

namespace s2 {

// ----------------------------------------------------------------------------------------------
inline 	void dumpTGA(const std::string &targaFilename, unsigned char *pixels, short numChannels, short w, short h)
{
	//typedef struct {
	//   //char  idlength;
	//   //char  colourmaptype;
	//   //char  datatypecode;
	//   //short int colourmaporigin;
	//   //short int colourmaplength;
	//   //char  colourmapdepth;
	//   //short int x_origin;
	//   //short int y_origin;
	//   //short width;
	//   //short height;
	//   //char  bitsperpixel;
	//   //char  imagedescriptor;
	//	GLubyte Header[12]; //12 bytes
	//} TGA_HEADER;

	const char cTgaHeader[12] = { 0, 0, 2, 0,  0, 0, 0, 0,  0, 0, 0, 0 };

	std::ofstream file;

	// Open the file for output
	file.open(targaFilename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc); 

	if (!file)
		return;

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
}


}
#endif