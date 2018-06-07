// ImageBuffer.h
//
#ifndef CORE_IMAGEBUFFER_H
#define CORE_IMAGEBUFFER_H

#include <string>
#include <memory>

namespace s2 {

template<typename T>
class ImageBuffer
{
public:
	static std::shared_ptr<ImageBuffer<T>> New() { return std::make_shared<ImageBuffer<T>>(); }
	static std::shared_ptr<ImageBuffer<T>> New( int width, int height, int channels, T *pxl = nullptr ) { return std::make_shared<ImageBuffer<T>>( width, height, channels, pxl ); }

public:
    ImageBuffer();
    ImageBuffer( int width, int height, int channels, T *pxl = nullptr );
	~ImageBuffer();
    // @todo: copy constructor, assignment

    void setValues( T *values ); 
	void resize( int width, int height, int channels );
    void destroy();
	void clear();

    T   &operator[](int i);
    T   *pixels()      const { return _data; }
	int  numChannels() const { return _numCh; }
	int  width()       const { return _w; }
	int  height()      const { return _h; }
	bool isEmpty()     const { return _data == 0; }

	bool dump(const std::string &tgaFilename) const;

private:
	void create( int width, int height, int channels, T *pxl );

private:
    int	_w;
    int _h;
    int _numCh;
    T   *_data;
};

template< typename T >
using ImageBufferPtr = std::shared_ptr< ImageBuffer<T> >;

}

#include "ImageBuffer.hpp"

#endif