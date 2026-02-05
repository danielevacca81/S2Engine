// ColorMap.h
//
#ifndef CORE_COLORMAP_H
#define CORE_COLORMAP_H

#include "s2Engine_API.h"

#include "Color.h"

#include <vector>

namespace s2 {

class S2ENGINE_API ColorMap
{
public:
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
	static ColorMap build( const std::vector< ColorEntry > &entries, const uint32_t &resolution = 256 );


public:
	ColorMap( const std::vector< Color > &colors = {} );

	void                        setColors( const std::vector< Color > &colors );
	const std::vector< Color > &colors()      const;
	int                         colorsCount() const;

	bool                        operator==( const ColorMap &o ) const { return _colors == o._colors; }
	bool                        operator!=( const ColorMap &o ) const { return ! operator==(o); }

private:
	std::vector< Color > _colors;
};

}

#endif