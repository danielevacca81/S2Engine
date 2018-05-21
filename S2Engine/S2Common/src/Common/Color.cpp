// Color.cpp
//
#include "Color.h"

#include <memory.h>

using namespace s2;

// ------------------------------------------------------------------------------------------------
Color::Color() 
: _r(.5f)
, _g(.5f)
, _b(.5f)
, _a(1.f)
{}

// ------------------------------------------------------------------------------------------------
Color::Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A )
: _r(R/255.f)
, _g(G/255.f)
, _b(B/255.f)
, _a(A/255.f)
{}

// ------------------------------------------------------------------------------------------------
Color::Color(float R, float G, float B, float A = 1.f)
: _r(R)
, _g(G)
, _b(B)
, _a(A)
{}
	
// ------------------------------------------------------------------------------------------------
float Color::r()         const { return _r; }
float Color::g()         const { return _g; }
float Color::b()         const { return _b; }
float Color::a()         const { return _a; }
float *Color::rgba()           { return &_r;}
float Color::luminance() const { return (_r*0.2126f)+(_g*0.7152f)+(_b*0.0722f); }
	
// ------------------------------------------------------------------------------------------------
Color Color::lighter( float b ) const 
{ 
	const float br = _r+b > 1.0f ? 1.0f : _r+b;
	const float bg = _g+b > 1.0f ? 1.0f : _g+b;
	const float bb = _b+b > 1.0f ? 1.0f : _b+b;

	return Color(br,bg,bb,_a);
}

// ------------------------------------------------------------------------------------------------
Color Color::darker( float b ) const 
{ 
	const float br = _r-b < 0.0f ? 0.0f : _r-b;
	const float bg = _g-b < 0.0f ? 0.0f : _g-b;
	const float bb = _b-b < 0.0f ? 0.0f : _b-b;

	return Color(br,bg,bb,_a);
}

// ------------------------------------------------------------------------------------------------
Color Color::transparent( float alpha ) const 
{ 
	return Color(_r,_g,_b,_a*alpha);
}

// ------------------------------------------------------------------------------------------------
bool Color::equals( const Color &other ) const 
{ 
	return memcmp(&_r,&other._r,sizeof(float)*4) == 0;
}