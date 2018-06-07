// Geometry.h
// 
#ifndef GEOMETRY_ONCE
#define GEOMETRY_ONCE

#include "s2Core_API.h"

#include "Math/Math.h"

#include <vector>

namespace s2 {

namespace Geometry
{

// 2D 
//std::vector< Math::dvec3 > S2CORE_API ellipse();
//std::vector< Math::dvec3 > S2CORE_API rectangle();
std::vector< Math::dvec3 > S2CORE_API circle( const Math::dvec3 &center, double radius, int LOD = 32 );


// 3D
//std::vector< Math::dvec3 > S2CORE_API cube();
//std::vector< Math::dvec3 > S2CORE_API sphere( int numc, int numt );
//std::vector< Math::dvec3 > S2CORE_API cylinder( int numc, int numt );
//std::vector< Math::dvec3 > S2CORE_API cone( int numc, int numt );
std::vector< Math::dvec3 > S2CORE_API torus( int numc, int numt );
//VMesh S2CORE_API teapot( int resU, int resV );

}
}

#endif