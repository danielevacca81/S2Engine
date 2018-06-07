// Frustum.h
//
#ifndef FRUSTUM_ONCE
#define FRUSTUM_ONCE

#include "Math/Plane.h"
#include "Math/Box.h"

#include <vector>

namespace OpenGL{

class Frustum
{
private:
	std::vector<Math::dplane> _planes;

public:
	enum TestResult {FrustumInside, FrustumOutside, FrustumIntersect};

	// -------------------------------------------------------------------------------------------------
	Frustum() : _planes(6) {}
	
	// -------------------------------------------------------------------------------------------------
	~Frustum() {}

	// -------------------------------------------------------------------------------------------------
	void update( const Math::dmat4 &modelView, const Math::dmat4 &projection );

	TestResult isInside( const Math::box3 &b ) const;
	TestResult isInside( const Math::dvec2 &p, double radius ) const;
	TestResult isInside( const Math::dvec2 &p ) const;
	TestResult isInside( const Math::dvec3 &p, double radius ) const ;
	TestResult isInside( const Math::dvec3 &p ) const;
};


}
#endif