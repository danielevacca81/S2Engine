// Geometry.h
// 
#ifndef GEOMETRY_ONCE
#define GEOMETRY_ONCE

#include "s2Engine_API.h"

#include "Math/Math.h"
#include "Mesh.h"

#include <vector>

namespace s2 {

class S2ENGINE_API GeometryFactory2D
{
public:

// 2D 
//std::vector< Math::dvec3 > S2ENGINE_API ellipse();
//std::vector< Math::dvec3 > S2ENGINE_API rectangle();
std::vector< Math::dvec2 >  circle( const Math::dvec2 &center, double radius, int slices = 32 );


private:
	GeometryFactory2D() = delete;
};
// 3D
//std::vector< Math::dvec3 > S2ENGINE_API cube();
//std::vector< Math::dvec3 > S2ENGINE_API sphere( int numc, int numt );
//std::vector< Math::dvec3 > S2ENGINE_API cylinder( int numc, int numt );
//std::vector< Math::dvec3 > S2ENGINE_API cone( int numc, int numt );
//Mesh S2ENGINE_API torus( double innerRadius, double outerRadius, int numc, int numt );
//Math::Mesh S2ENGINE_API torus( double innerRadius, double outerRadius, int sidesCount=64, int ringsCount=32 );
//Math::Mesh S2ENGINE_API cylinder( const Math::dvec3 &startPoint, const Math::dvec3 &endPoint, double radius, bool capStart, bool capEnd, int slices = 32 );
//Math::Mesh S2ENGINE_API sphere( const Math::dvec3 &center, double radius, int slices = 32 );

//VMesh S2ENGINE_API teapot( int resU, int resV );

}

#endif