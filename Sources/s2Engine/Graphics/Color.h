// Color.h
//
#ifndef CORE_COLOR_H
#define CORE_COLOR_H

#include "s2Engine_API.h"

#include <cstdint>

namespace s2 {

class S2ENGINE_API Color
{

public:
	struct S2ENGINE_API HSL
	{
		float hue        = 0.f;
		float saturation = 0.f;
		float lightness  = 0.f;
	};

	struct S2ENGINE_API HSV
	{
		float hue        = 0.f;
		float saturation = 0.f;
		float value      = 0.f;
	};

public:
	static Color fromHex( uint32_t hexValue );
	static Color fromHex64( uint64_t hexValue64 );
	static Color fromHSV( const HSV& hsv );
	static Color fromHSL( const HSL& hsl );
	static Color fromBytes( uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255 );

public:
	Color() = default;
	explicit Color( float R, float G, float B, float A );
	explicit Color( float R, float G, float B );
		
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
	// Colors by name
	static constexpr Color white()   noexcept { return Color(1.f,1.f,1.f,1.f); }
	static constexpr Color black()   noexcept { return Color(0.f,0.f,0.f,1.f); }
	static constexpr Color red()     noexcept { return Color(1.f,0.f,0.f,1.f); }
	static constexpr Color green()   noexcept { return Color(0.f,1.f,0.f,1.f); }
	static constexpr Color blue()    noexcept { return Color(0.f,0.f,1.f,1.f); }
	static constexpr Color yellow()  noexcept { return Color(1.f,1.f,0.f,1.f); }
	static constexpr Color cyan()    noexcept { return Color(0.f,1.f,1.f,1.f); }
	static constexpr Color magenta() noexcept { return Color(1.f,0.f,1.f,1.f); }
	static constexpr Color gray()    noexcept { return Color(.5f,.5f,.5f,1.f); }
	static constexpr Color orange()  noexcept { return Color(1.f,.6f,.0f,1.f); }
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

// ------------------------------------------------------------------------------------------------
// Non-member operator for scalar pre-multiplication
inline Color operator*( float v, const Color& c ) { return c * v; }

}
#endif