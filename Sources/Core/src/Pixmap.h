// Pixmap.h
//
#ifndef CORE_PIXMAP_H
#define CORE_PIXMAP_H

#include <string>
#include <memory>
#include <vector>

template<typename T>
class Pixmap
{
public:
    Pixmap() = default;
    Pixmap( int32_t width, int32_t height, int32_t channels, T *pxl = nullptr );

    void setPixels( T *values ); 
	void resize( int32_t width, int32_t height, int32_t channels );
	void clear();

    T        &operator[](int i) const ;
    const T  *pixels()          const { return &_pixels[0]; }
	uint32_t  numChannels()     const { return _numCh; }
	uint32_t  width()           const { return _w; }
	uint32_t  height()          const { return _h; }
    bool      isEmpty()         const { return _pixels.empty(); }

	bool dump(const std::string &tgaFilename) const;

private:
    uint32_t _w     = 0;
    uint32_t _h     = 0;
    uint32_t _numCh = 0;
    std::vector<T> _pixels;
};

#include "Pixmap.hpp"

#endif