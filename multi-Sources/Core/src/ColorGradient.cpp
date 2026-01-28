// ColorGradient.cpp
//
#include "ColorGradient.h"

// ------------------------------------------------------------------------------------------------
ColorGradient::ColorGradient( const Color &startColor, const Color &endColor, const Type &t )
: ColorGradient( t )
{
	_keyColors[0.f] = startColor;
	_keyColors[1.f] = endColor;
}

// ------------------------------------------------------------------------------------------------
void                ColorGradient::setType( const Type &t )       { _type = t;    }
ColorGradient::Type ColorGradient::type()                   const { return _type; }

// ------------------------------------------------------------------------------------------------
void  ColorGradient::addColorAt( float pos, const Color &color )
{
	if( pos < 0.f ) pos = 0.f;
	if( pos > 1.f ) pos = 1.f;

	_keyColors[pos] = color;
}

// ------------------------------------------------------------------------------------------------
Color ColorGradient::colorAt( float pos ) const
{
	if( pos < 0.f ) pos = 0.f;
	if( pos > 1.f ) pos = 1.f;

	// if required position is not present
	if( pos <= _keyColors.begin()->first  ) return _keyColors.begin()->second;
	if( pos >= _keyColors.rbegin()->first ) return _keyColors.rbegin()->second;


	switch( _type )
	{
	case GradientLinear:
	default:
		// extract range
		const auto a = _keyColors.lower_bound( pos );
		const auto b = std::next( a );

		// normalize pos into b-a range
		const float range = b->first - a->first;
		const float t     = ( pos - a->first ) / range;

		return a->second * ( 1.f -t ) + ( b->second * t);
	}

	return _keyColors.begin()->second;
}