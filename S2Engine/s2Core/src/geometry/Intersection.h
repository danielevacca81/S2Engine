// Intersection.h
//
#ifndef INTERSECTION_ONCE
#define INTERSECTION_ONCE

#include "s2Core_API.h"

#include "Math/Math.h"
#include "Math/Box.h"

namespace glm {

/// Line - line intersection in 2D
bool S2CORE_API lineIntersection
(
	const dvec2 &a1,
	const dvec2 &a2,
	const dvec2 &b1,
	const dvec2 &b2,
	dvec2 *out=0
);

bool S2CORE_API lineBoxIntersection(
	const dvec3 &p0,
	const dvec3 &p1,
	const box3 &b );

/// Cross product for 2D vectors
double cross( const dvec2& a, const dvec2& b );

/// Check if two points p1, p1 are on the same side of line a-b (in 2D)
bool S2CORE_API pointsOnSameSideOfLine
(
	const dvec2 &p1,
	const dvec2 &p2,
	const dvec2 &a,
	const dvec2 &b
);

/// Check a point belongs to triangle in 2D
bool S2CORE_API pointBelongsToTriangle
(
	const dvec2 &p,
	const dvec2 &a1,
	const dvec2 &a2,
	const dvec2 &a3
);

/// Check 2 triangles intersects in 2D
bool S2CORE_API triangles2DIntersection
(
	const dvec2 &a1,
	const dvec2 &a2,
	const dvec2 &a3,
	const dvec2 &b1,
	const dvec2 &b2,
	const dvec2 &b3
);

/// Check if a triangle intersects a box in 2D (even if box3 is 3D)
bool S2CORE_API triangleBox2DIntersection
(
	const dvec2 &a1,
	const dvec2 &a2,
	const dvec2 &a3,
	const box3  &b
);

}

#endif