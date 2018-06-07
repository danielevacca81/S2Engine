// Rectangle.h
// 
#ifndef RECTANGLE_ONCE
#define RECTANGLE_ONCE

#include "Math/Math.h"

namespace glm {

class Rectangle
{
private:
	int _left,_bottom;
	int _width,_height;

public:
	// ------------------------------------------------------------------------------------------------
	Rectangle( int l, int b, int w, int h ) 
	: _left(l), _bottom(b), _width(w), _height(h)
	{}

	// ------------------------------------------------------------------------------------------------
	Rectangle() 
	: _left(0), _bottom(0), _width(0), _height(0)
	{}

	// ------------------------------------------------------------------------------------------------
	bool equals( const Rectangle &other ) const 
	{ 
		return _left   == other._left && 
			   _bottom == other._bottom &&
		       _width  == other._width &&
			   _height == other._height;
	}

	// ------------------------------------------------------------------------------------------------
	int left()   const { return _left;           }
	int width()  const { return _width;          }
	int height() const { return _height;         }
	int bottom() const { return _bottom;         }
	int top()    const { return _bottom+_height; }
	int right()  const { return _left+_width;    }
};


}

#endif