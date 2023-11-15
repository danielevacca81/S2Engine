// Color.cpp
//
#include "Color.h"

#include <algorithm>
#include <cmath>


// ------------------------------------------------------------------------------------------------
Color::Color(float R, float G, float B, float A )
: _r(R)
, _g(G)
, _b(B)
, _a(A)
{}

// ------------------------------------------------------------------------------------------------
float Color::r()           const { return _r; }
float Color::g()           const { return _g; }
float Color::b()           const { return _b; }
float Color::a()           const { return _a; }
const float *Color::rgba() const { return &_r;}

// ------------------------------------------------------------------------------------------------
float Color::luminance()  const { return (_r*0.2126f)+(_g*0.7152f)+(_b*0.0722f); }
float Color::brightness() const { return (((_r*0.299f)+(_g*0.587f)+(_b*0.114f))*255.f); }
float Color::hue() const
{
	const float mx = std::max( { _r, _g,_b } );
	const float mn = std::min( { _r, _g,_b } );
		
	float h = 0;
	if     ( mx == _r ) h = 0.f + (_g-_b);
	else if( mx == _g ) h = 2.f + (_b-_r);
	else if( mx == _b ) h = 4.f + (_r-_g);			
		
	h = h*60.f/(mx-mn);

	if(h < 0.f) h = h + 360.f;
		
	return h;
}
	
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
Color Color::opaque() const // [ convenient method ] same as transparent(1.f)
{ 
	return Color(_r,_g,_b,1.f);
}

// ------------------------------------------------------------------------------------------------
Color Color::fromBytes( unsigned char R, unsigned char G, unsigned char B, unsigned char A )
{
	return Color( R / 255.f, G / 255.f, B / 255.f, A / 255.f );
}

// ------------------------------------------------------------------------------------------------
Color Color::fromHSV( const HSV &hsv )
{
	const float h = hsv.hue / 60.f;
	const float s = hsv.saturation;
	const float v = hsv.value;

	if( s <= 0.f ) 
		return Color(v,v,v); // achromatic gray
		
	const int i   = (int)std::floor(h); // sectors 0 to 5
	const float f = h - i;        // fract
	const float p = hsv.value * (1.f - s);
	const float q = hsv.value * (1.f - (s * f));
	const float t = hsv.value * (1.f - (s * (1.f - f)));

	switch(i)
	{
	case 0:  return Color(v,t,p);
	case 1:  return Color(q,v,p);
	case 2:  return Color(p,v,t);
	case 3:  return Color(p,q,v);
	case 4:  return Color(t,p,v);
	}
		
	// default
	return Color(v,p,q);
}

// ------------------------------------------------------------------------------------------------
Color Color::fromHSL( const HSL &hsl )
{
	const float h = hsl.hue/60.f;
	const float s = hsl.saturation;
	const float l = hsl.lightness;

	const float v = (l < 0.5f) ? (l * (1.f + s)) : (l + s - l * s);

	if( v <= 0.f )
		return Color(l,l,l); // achromatic gray

	const float m     = l + l - v;
	const float sv    = (v - m ) / v;
	const int sextant = (int)std::floor(h);
	const float fract = h - sextant;
	const float vsf   = v * sv * fract;
	const float mid1  = m + vsf;
	const float mid2  = v - vsf;

	Color outColor;
	switch( sextant )
	{
	case 0: outColor = Color(v,mid1,m); break;
	case 1: outColor = Color(mid2,v,m); break;
	case 2: outColor = Color(m,v,mid1); break;
	case 3: outColor = Color(m,mid2,v); break;
	case 4: outColor = Color(mid1,m,v); break;
	case 5: outColor = Color(v,m,mid2); break;
	}

	return outColor;
}

// ------------------------------------------------------------------------------------------------
Color Color::blend( const Color &c1, float u ) const
{
	Color outColor
	{
		_r * ( 1.f - u ) + c1._r * u,
		_g * ( 1.f - u ) + c1._g * u,
		_b * ( 1.f - u ) + c1._b * u,
		_a * ( 1.f - u ) + c1._a * u,
	};

	return outColor;
}

// ------------------------------------------------------------------------------------------------
Color::HSL Color::toHSL() const
{
	const float max = std::max( {_r, _g,_b} );
	const float min = std::min( {_r, _g,_b} );
	const float d   = max-min;		
	const float l   = (max+min)*.5f;
		
	if( d <= 1e-9 )
		return HSL { 0.f, 0.f, l }; // achromatic

	const float s = l > .5f ? d / (2.f - max - min) : d / (max + min);
	const float h = hue();

	return HSL { h,s,l };
}

// ------------------------------------------------------------------------------------------------
Color::HSV Color::toHSV() const
{
	const float mx = std::max({_r, _g,_b});
	const float mn = std::min({_r, _g,_b});
	const float d   = mx-mn;
		
	const float s = mx > 0.f ? d / mx : 0.f;
	const float v = mx;
	const float h = d > 1e-9 ? hue() : 0.f;

	return HSV { h,s,v };
}
	
// ------------------------------------------------------------------------------------------------
Color Color::bestMatch() const
{
	HSL hsl = toHSL();

	Color out;
	if( hsl.saturation >= .3f )
	{
		// choose opposite hue with max saturation and lightness
		hsl.hue        = std::fmod(hsl.hue + 180.f,360.f);
		hsl.saturation = 1.f;
		hsl.lightness  = .5f;
			
		out = Color::fromHSL(hsl);
	}
	else
	{
		// for low saturation use black or white
		if( hsl.lightness >= .5f ) out = black();
		else                       out = white();
	}

	return out;
}

// ------------------------------------------------------------------------------------------------
Color Color::random()
{
	const auto r = std::rand() % std::numeric_limits<uint8_t>::max();
	const auto g = std::rand() % std::numeric_limits<uint8_t>::max();
	const auto b = std::rand() % std::numeric_limits<uint8_t>::max();
		
	return fromBytes( r,g,b, 255 );
}

// ------------------------------------------------------------------------------------------------
Color Color::operator +( const Color &c ) const
{
	return Color( std::min( 1.f, _r + c._r ),
				  std::min( 1.f, _g + c._g ),
				  std::min( 1.f, _b + c._b ),
				  std::min( 1.f, _a + c._a )
	);
}

// ------------------------------------------------------------------------------------------------
Color Color::operator -( const Color& c ) const
{
	return Color( std::max( 0.f, _r - c._r ),
				  std::max( 0.f, _g - c._g ),
				  std::max( 0.f, _b - c._b ),
				  std::max( 0.f, _a - c._a )
	);
}


// ------------------------------------------------------------------------------------------------
Color Color::operator *( const float& v ) const
{
	return Color( std::min( 1.f, _r * v ),
				  std::min( 1.f, _g * v ),
				  std::min( 1.f, _b * v ),
				  std::min( 1.f, _a * v )
	);
}

// ------------------------------------------------------------------------------------------------
bool Color::operator==( const Color &c ) const
{
	return _r == c._r &&
	       _g == c._g &&
		   _b == c._b &&
		   _a == c._a ;
}

// ------------------------------------------------------------------------------------------------
bool Color::operator!=( const Color &c ) const
{
	return !operator==( c );
}