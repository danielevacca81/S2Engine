// Gradient.h
//
#ifndef SCENE_GRADIENT_ONCE
#define SCENE_GRADIENT_ONCE

#include "s2Scene_API.h"

#include "Core/Color.h"

#include <map>

namespace Scene {

class S2SCENE_API Gradient
{
public:
	enum Type
	{
		GradientLinear,
	};

public:
	Gradient() = default;
	Gradient( const Type &t ): _type( t ) {}
	Gradient( const Color &startColor, const Color &endColor, const Type &t );
	
	void  addColorAt( float pos, const Color &color );
	Color colorAt( float pos ) const;
	
	Color colorAtStart() const { return colorAt( 0.f ); }
	Color colorAtEnd()   const { return colorAt( 1.f ); }

	void  setType( const Type &t );
	Type  type() const;


protected:
	Type                  _type;
	std::map<float,Color> _keyColors;

	// @todo_dv: add directions?
};

}

#endif // !SCENE_GRADIENT_ONCE

