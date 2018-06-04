// Image.h
//
#ifndef IMAGE_ONCE
#define IMAGE_ONCE

#include "CoreAPI.h"


#include <string>

namespace OpenGL {

class S2CORE_API Image
{
private:
	int		w,h,numCh;
	void	*data;

public:
	Image();
	Image( int width, int height, int channels, void *pxl );
	~Image();

	void create( int width, int height, int channels, void *pxl );
	void clear();

	bool isEmpty()		const { return data == 0; }

	void *pixels()		const { return data; }
	int  numChannels()	const { return numCh; }
	int  width()		const { return w; }
	int  height()		const { return h; }

	bool dump(const std::string &tgaFilename);
};

}


#endif