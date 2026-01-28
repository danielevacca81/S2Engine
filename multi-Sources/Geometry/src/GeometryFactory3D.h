// GeometryFactory.h
// 
#ifndef GEOMETRY_GEOMETRYFACTORY3D_ONCE
#define GEOMETRY_GEOMETRYFACTORY3D_ONCE

#include "Geometry_API.h"

#include "Geometry.h"

#include "Math/Math.h"

#include <vector>

namespace Geometry {

class GEOMETRY_API GeometryFactory3D
{
public:
	static Geometry createTorus( double innerRadius, double outerRadius, int sidesCount, int ringsCount );

	// static Mesh cylinder( const Math::dvec3 &startPoint, const Math::dvec3 &endPoint, double radius, bool capStart, bool capEnd, int slices );
	// static Mesh sphere( const Math::dvec3 &center, double radius, int slices );
	// ...
	// static Mesh cone( const Math::dvec3 &startPoint, const Math::dvec3 &endPoint, double radius, bool capStart, bool capEnd, int slices );
	// static Mesh cube( const Math::dvec3 &center, double size );
	// static Mesh teapot( int resU, int resV );
	// ...

public:
	GeometryFactory3D() = delete;
};

}

#endif