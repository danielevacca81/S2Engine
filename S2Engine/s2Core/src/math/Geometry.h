// Geometry.h
// 
#ifndef GEOMETRY_ONCE
#define GEOMETRY_ONCE

#include "s2Core_API.h"

#include "Math.h"
#include "Mesh.h"

#include <vector>

namespace s2 {

// 2D 
//std::vector< Math::dvec3 > S2CORE_API ellipse();
//std::vector< Math::dvec3 > S2CORE_API rectangle();
std::vector< Math::dvec2 > S2CORE_API circle( const Math::dvec2 &center, double radius, int LOD = 32 );


// 3D
//std::vector< Math::dvec3 > S2CORE_API cube();
//std::vector< Math::dvec3 > S2CORE_API sphere( int numc, int numt );
//std::vector< Math::dvec3 > S2CORE_API cylinder( int numc, int numt );
//std::vector< Math::dvec3 > S2CORE_API cone( int numc, int numt );
//Mesh S2CORE_API torus( double innerRadius, double outerRadius, int numc, int numt );
Math::Mesh S2CORE_API torus( double innerRadius, double outerRadius, int sidesCount=64, int ringsCount=32 );
//VMesh S2CORE_API teapot( int resU, int resV );

}

#endif