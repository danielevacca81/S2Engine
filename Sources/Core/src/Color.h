// Color.h
//
#ifndef CORE_COLOR_H
#define CORE_COLOR_H

#include "Core_API.h"

class CORE_API Color
{

public:
	struct CORE_API HSL
	{
		float hue        = 0.f;
		float saturation = 0.f;
		float lightness  = 0.f;
	};

	struct CORE_API HSV
	{
		float hue        = 0.f;
		float saturation = 0.f;
		float value      = 0.f;
	};

public:
	Color() = default;
	explicit Color( float R, float G, float B, float A = 1.f );
		
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

	Color operator +( const Color &c ) const;
	Color operator -( const Color &c ) const;
	Color operator *( const float &f ) const;
	bool operator==( const Color &c ) const;
	bool operator!=( const Color &c ) const;

private:
	float _r = 0.f;
	float _g = 0.f;
	float _b = 0.f;
	float _a = 1.f;
};

#endif