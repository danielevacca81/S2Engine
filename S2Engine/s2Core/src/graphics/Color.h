// Color.h
//
#ifndef CORE_COLOR_H
#define CORE_COLOR_H

#include "s2Core_API.h"

#include "math/Math.h"

namespace s2 {

class S2CORE_API Color
{

public:
	struct S2CORE_API HSL
	{
		float hue;
		float saturation;
		float lightness;
		HSL( float h,float s, float l ) : hue(h),saturation(s),lightness(l) {}
	};

	struct S2CORE_API HSV
	{
		float hue;
		float saturation;
		float value;
		HSV( float h,float s, float v ) : hue(h),saturation(s),value(v) {}
	};

	// ------------------------------------------------------------------------------------------------
	Color();
	Color( float R, float G, float B, float A = 1.f );
	explicit Color( unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255 );
		
	// ------------------------------------------------------------------------------------------------
	float r() const;
	float g() const;
	float b() const;
	float a() const;
	const float *rgba() const;

	// ------------------------------------------------------------------------------------------------
	float luminance()  const;
	float brightness() const;
	float hue() const;
	
	// ------------------------------------------------------------------------------------------------
	Color lighter( float b = 0.5f ) const;
	Color darker( float b = 0.5f ) const;
	Color transparent( float alpha = 0.5f ) const;
	Color opaque() const; // [ convenient method ] same as transparent(1.f)

	// ------------------------------------------------------------------------------------------------
	HSL toHSL() const;
	HSV toHSV() const;
	
	// ------------------------------------------------------------------------------------------------
	Color blend( const Color &c1, float u = .5f ) const;
	Color bestMatch() const;

	// ------------------------------------------------------------------------------------------------
	static Color fromHSV( const HSV &hsv );
	static Color fromHSL( const HSL &hsl );
	static Color fromBytes( unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255 );

	// ------------------------------------------------------------------------------------------------
	// Colors by name
	static Color white()   { return Color(1.f,1.f,1.f,1.f); }
	static Color black()   { return Color(0.f,0.f,0.f,1.f); }
	static Color red()	   { return Color(1.f,0.f,0.f,1.f); }
	static Color green()   { return Color(0.f,1.f,0.f,1.f); }
	static Color blue()    { return Color(0.f,0.f,1.f,1.f); }
	static Color yellow()  { return Color(1.f,1.f,0.f,1.f); }
	static Color cyan()    { return Color(0.f,1.f,1.f,1.f); }
	static Color magenta() { return Color(1.f,0.f,1.f,1.f); }
	static Color gray()    { return Color(.5f,.5f,.5f,1.f); }
	static Color orange()  { return Color(1.f,.6f,.0f,1.f); }
	static Color random();


	// ------------------------------------------------------------------------------------------------
	// Cast Operator
	operator float const *()       { return &_r; }
	operator Math::vec4 const( )   { return Math::vec4(_r,_g,_b,_a); }

	Color operator +( const Color &c ) const;
	bool operator==( const Color &c ) const;
	bool operator!=( const Color &c ) const;

private:
	float _r,_g,_b,_a;
};

}

#endif