// Rectangle.h
// 
#ifndef RECTANGLE_ONCE
#define RECTANGLE_ONCE

#include "Math/Math.h"

namespace glm {

class Rectangle// templated and class name lowercase? irect,frect,drect
{
public:
	// ------------------------------------------------------------------------------------------------
	Rectangle() 
	: _left(0), _bottom(0), _width(0), _height(0)
	{}

	// ------------------------------------------------------------------------------------------------
	Rectangle( int l, int b, int w, int h ) 
	: _left(l), _bottom(b), _width(w), _height(h)
	{}

	// ------------------------------------------------------------------------------------------------
	Rectangle( const ivec2 &p0, const ivec2 &p1 ) 
	{
		_left   = p0.x < p1.x ? p0.x : p1.x;
		_bottom = p0.y < p1.y ? p0.y : p1.y;
		_width  = glm::abs( p1.x - p0.x );
		_height = glm::abs( p1.y - p0.y );
	}

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

private:
	int _left,_bottom;
	int _width,_height;
};


}

#endif