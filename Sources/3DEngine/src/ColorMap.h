// ColorMap.h
//
#ifndef SCENE_COLORMAP_ONCE
#define SCENE_COLORMAP_ONCE

#include "s2Scene_API.h"

#include "Core/Color.h"

#include <vector>

namespace Scene {

class S2SCENE_API ColorMap
{
public:
	enum class Size
	{
		Size_256  = 256,
		Size_512  = 512,
		Size_1024 = 1024,
	};

	struct ColorEntry
	{
		ColorEntry( float pos, const Color &col, bool grad )
			: position( pos )
			, color( col )
			, gradient( grad )
		{}

		float   position;
		bool    gradient;
		Color   color;
	};


public:
	static ColorMap build( const std::vector< ColorEntry > &entries, const Size &size = Size::Size_1024 );


public:
	ColorMap( const std::vector< Color > &colors = {} );

	void                        setColors( const std::vector< Color > &colors );
	const std::vector< Color > &colors()      const;
	int                         colorsCount() const;

	bool                        operator==( const ColorMap &o ) const { return _colors == o._colors; }
	bool                        operator!=( const ColorMap &o ) const { return ! operator==(o); }

	//OpenGL::Texture1DPtr toTexture() const;

private:
	std::vector< Color > _colors;
};

}

#endif