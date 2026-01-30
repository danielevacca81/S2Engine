// Geometry.cpp
// 
#include "Geometry.h"

namespace s2 {
namespace Geometry {

// ------------------------------------------------------------------------------------------------
std::vector< Math::dvec2 > circle( const Math::dvec2 &center, double radius, int LOD /* = 32 */ )
{
	std::vector< Math::dvec2 > circlePoints;

	const double step = Math::two_pi<double>() / LOD;

	circlePoints.push_back( Math::dvec2( center ) ); // center as first point

	for( int i=0; i <= LOD; ++i )
	{
		const double angle  = i * step;
		const Math::dvec2 p =  Math::dvec2( Math::cos( angle ), Math::sin( angle ) )*radius;

		circlePoints.push_back( center + p );
	}

	return circlePoints;
}

}
}