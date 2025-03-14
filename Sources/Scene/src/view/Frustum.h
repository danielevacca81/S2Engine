// Frustum.h
//
#ifndef RENDERENGINE_FRUSTUM_H
#define RENDERENGINE_FRUSTUM_H

#include "Math/Plane.h"
#include "Math/Box.h"

#include <array>

namespace RenderPipeline {

class Frustum
{
public:
	enum class TestResult { FrustumInside, FrustumOutside, FrustumIntersect };

public:
	void update( const Math::dmat4 &modelView, const Math::dmat4 &projection );

	// add triangle test
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