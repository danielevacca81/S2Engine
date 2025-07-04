// Geometry.h
//
#ifndef RENDERER_GEOMETRY_H
#define RENDERER_GEOMETRY_H

#include "Resource.h"


namespace Renderer{

class Surface; // Forward declaration

class Drawable
{
public:
	virtual void draw( Surface* ) const = 0;
};


class Geometry : public Drawable
{
public:
	Geometry() : Drawable()
	{}

	virtual ~Geometry() = default;

	void draw( Surface* surface ) const override
	{
		// Additional drawing logic specific to Geometry can be added here
		// For example, setting up vertex buffers, index buffers, etc.
	}
};

}

#endif // !RENDERER_GEOMETRY_H