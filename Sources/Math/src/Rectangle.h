// Rectangle.h
// 
#ifndef RECTANGLE_ONCE
#define RECTANGLE_ONCE

#include "Math.h"

namespace glm {
namespace detail {

// Rectangle class (bottom left) [ top > bottom and right > left]
template<typename T, precision P>
class trect
{
public:
	// ------------------------------------------------------------------------------------------------
	trect() = default;

	// ------------------------------------------------------------------------------------------------
	trect( T l, T b, T w, T h )
		: _left( l ), _bottom( b ), _width( w ), _height( h )
	{}

	// ------------------------------------------------------------------------------------------------
	trect( const tvec2<T, P>& p0, const tvec2<T, P>& p1 )
	{
		_left = p0.x < p1.x ? p0.x : p1.x;
		_bottom = p0.y < p1.y ? p0.y : p1.y;
		_width = glm::abs( p1.x - p0.x );
		_height = glm::abs( p1.y - p0.y );
	}

	// ------------------------------------------------------------------------------------------------
	bool equals( const trect& other ) const
	{
		return _left == other._left &&
			_bottom == other._bottom &&
			_width == other._width &&
			_height == other._height;
	}

	// ------------------------------------------------------------------------------------------------
	bool isEmpty() const
	{
		return _left <= 0 &&
			_bottom <= 0 &&
			_width <= 0 &&
			_height <= 0;
	}

	// ------------------------------------------------------------------------------------------------
	bool contains( const tvec2<T, P>& p ) const
	{
		return p.x >= left() && p.x <= right() &&
			p.y >= bottom() && p.y <= top();
	}

	// ------------------------------------------------------------------------------------------------
	inline T left()   const { return _left; }
	inline T width()  const { return _width; }
	inline T height() const { return _height; }
	inline T bottom() const { return _bottom; }
	inline T top()    const { return _bottom + _height; }
	inline T right()  const { return _left + _width; }

	// ------------------------------------------------------------------------------------------------
	trect<T, P> extended( const T ptX, const T ptY ) const
	{
		const T l = glm::min( left(), ptX );
		const T r = glm::max( right(), ptX );
		const T b = glm::min( bottom(), ptY );
		const T t = glm::max( top(), ptY );
		return Rectangle( l, b, r - l, t - b );
	}

	// ------------------------------------------------------------------------------------------------
	trect<T, P> extended( const trect& rect2 ) const
	{
		if( isEmpty() )
			return rect2;
		else if( rect2.isEmpty() )
			return *this;
		else
		{
			const T l = glm::min( left(), rect2.left() );
			const T r = glm::max( right(), rect2.right() );
			const T b = glm::min( bottom(), rect2.bottom() );
			const T t = glm::max( top(), rect2.top() );
			return Rectangle( l, b, r - l, t - b );
		}
	}

	// ------------------------------------------------------------------------------------------------
	trect<T, P> buffered( const T leftBuffer, const T rightBuffer,
						  const T bottomBuffer, const T topBuffer ) const
	{
		if( isEmpty() )
			return *this;
		else
			return trect( _left - leftBuffer, _bottom - bottomBuffer,
						  _width + leftBuffer + rightBuffer, _height + bottomBuffer + topBuffer );
	}

	// ------------------------------------------------------------------------------------------------
	trect<T, P> intersection( const trect<T, P>& rect2 ) const
	{
		if( isEmpty() || rect2.isEmpty() )
			return {};
		else
		{
			const T l = glm::max( left(), rect2.left() );
			const T r = glm::min( right(), rect2.right() );
			const T b = glm::max( bottom(), rect2.bottom() );
			const T t = glm::min( top(), rect2.top() );
			return trect( l, b, r - l, t - b );
		}
	}

private:
	T _left = 0;
	T _bottom = 0;
	T _width = 0;
	T _height = 0;
};
}

typedef detail::trect<float, defaultp>  frect;
typedef detail::trect<double, defaultp>  drect;
typedef detail::trect<int, defaultp>  irect;

}

#endif