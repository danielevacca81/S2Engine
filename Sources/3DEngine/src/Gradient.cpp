// Gradient.cpp
//
#include "Gradient.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
Gradient::Gradient( const Color &startColor, const Color &endColor, const Type &t )
: Gradient( t )
{
	_keyColors[0.f] = startColor;
	_keyColors[1.f] = endColor;
}

// ------------------------------------------------------------------------------------------------
void           Gradient::setType( const Type &t )       { _type = t;    }
Gradient::Type Gradient::type()                   const { return _type; }

// ------------------------------------------------------------------------------------------------
void  Gradient::addColorAt( float pos, const Color &color )
{
	if( pos < 0.f ) pos = 0.f;
	if( pos > 1.f ) pos = 1.f;

	_keyColors[pos] = color;
}

// ------------------------------------------------------------------------------------------------
Color Gradient::colorAt( float pos ) const
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

		return Math::mix( static_cast<Math::vec4>( a->second ), static_cast<Math::vec4>( b->second ), t );
	}

	return _keyColors.begin()->second;
}