// Color.h
//
#ifndef COLOR_ONCE
#define COLOR_ONCE

#include "S2CommonAPI.h"

namespace s2 {

class S2COMMON_API Color
{
private:
	float _r,_g,_b,_a;

public:
	Color();
	Color(float R, float G, float B, float A);
	explicit Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A);

	float r() const;
	float g() const;
	float b() const;
	float a() const;
	float *rgba();
	float luminance() const;
	
	Color lighter    ( float b = 0.5f     ) const;
	Color darker     ( float b = 0.5f     ) const;
	Color transparent( float alpha = 0.5f ) const;

	// ------------------------------------------------------------------------------------------------
	// Colors by name
	static Color white()  { return Color(1.f,1.f,1.f,1.f); }
	static Color black()  { return Color(0.f,0.f,0.f,1.f); }
	static Color red()	  { return Color(1.f,0.f,0.f,1.f); }
	static Color green()  { return Color(0.f,1.f,0.f,1.f); }
	static Color blue()   { return Color(0.f,0.f,1.f,1.f); }
	static Color yellow() { return Color(1.f,1.f,0.f,1.f); }
	static Color cyan()   { return Color(0.f,1.f,1.f,1.f); }
	static Color magenta(){ return Color(1.f,0.f,1.f,1.f); }
	static Color gray()   { return Color(.5f,.5f,.5f,1.f); }
	static Color orange() { return Color(1.f,.6f,.0f,1.f); }

	// ------------------------------------------------------------------------------------------------
	// Operators
	operator float *() { return &_r; }
	bool equals( const Color &other ) const;
};

}
#endif