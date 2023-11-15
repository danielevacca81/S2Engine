// ColorGradient.h
//
#ifndef CORE_GRADIENT_ONCE
#define CORE_GRADIENT_ONCE

#include "Core_API.h"

#include "Color.h"

#include <map>

class CORE_API ColorGradient
{
public:
	enum Type
	{
		GradientLinear,
	};

public:
	ColorGradient() = default;
	ColorGradient( const Type &t ): _type( t ) {}
	ColorGradient( const Color &startColor, const Color &endColor, const Type &t );
	
	void  addColorAt( float pos, const Color &color );
	Color colorAt( float pos ) const;
	
	Color colorAtStart() const { return colorAt( 0.f ); }
	Color colorAtEnd()   const { return colorAt( 1.f ); }

	void  setType( const Type &t );
	Type  type() const;

protected:
	Type                  _type { GradientLinear };
	std::map<float,Color> _keyColors;
};

#endif // !CORE_GRADIENT_ONCE

