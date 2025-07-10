// Frustum.h
//
#ifndef RENDERENGINE_FRUSTUM_H
#define RENDERENGINE_FRUSTUM_H

#include "Plane.h"
#include "Box.h"

#include <array>

namespace glm {

class Frustum
{
public:
	enum class TestResult { FrustumInside, FrustumOutside, FrustumIntersect };

public:
	void computePlanes( const Math::dmat4 &modelView, const Math::dmat4 &projection );

	// @todo_dv: add triangle test

	TestResult isInside( const Math::dbox3 &b )                const;
	TestResult isInside( const Math::dvec2 &p, double radius ) const;
	TestResult isInside( const Math::dvec2 &p )                const;
	TestResult isInside( const Math::dvec3 &p, double radius ) const;
	TestResult isInside( const Math::dvec3 &p )                const;


private:
	std::array<Math::dplane,6> _planes;
};

}
#endif