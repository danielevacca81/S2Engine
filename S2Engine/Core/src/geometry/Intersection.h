// Intersection.h
//
#ifndef INTERSECTION_ONCE
#define INTERSECTION_ONCE

#include "Core_API.h"

#include "Math/Math.h"
#include "Math/Box.h"

namespace glm {

/// Line - line intersection in 2D
bool CORE_API lineIntersection
(
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &b1,
	const Math::dvec2 &b2,
	Math::dvec2 *out=0
);

bool CORE_API lineBoxIntersection( 
	const Math::dvec3 &p0,
	const Math::dvec3 &p1, 
	const Math::box3 &b );

/// Cross product for 2D vectors
double cross(const Math::dvec2& a, const Math::dvec2& b);

/// Check if two points p1, p1 are on the same side of line a-b (in 2D)
bool CORE_API pointsOnSameSideOfLine
(
	const Math::dvec2 &p1,
	const Math::dvec2 &p2, 
	const Math::dvec2 &a,
	const Math::dvec2 &b
);

/// Check a point belongs to triangle in 2D
bool CORE_API pointBelongsToTriangle
(
	const Math::dvec2 &p,
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &a3
);

/// Check 2 triangles intersects in 2D
bool CORE_API triangles2DIntersection
(
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &a3,
	const Math::dvec2 &b1, 
	const Math::dvec2 &b2,
	const Math::dvec2 &b3
);

/// Check if a triangle intersects a box in 2D (even if box3 is 3D)
bool CORE_API triangleBox2DIntersection
(
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &a3,
	const Math::box3  &b
);

}

#endif