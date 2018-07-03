// Frustum.h
//
#ifndef FRUSTUM_ONCE
#define FRUSTUM_ONCE

#include "Math/Plane.h"
#include "Math/Box.h"

#include <vector>

namespace s2 {
namespace Renderer {

class Frustum
{
public:
	enum TestResult { FrustumInside, FrustumOutside, FrustumIntersect };

public:
	Frustum();
	~Frustum();

	void update( const Math::dmat4 &modelView, const Math::dmat4 &projection );

	// add triangle test
	TestResult isInside( const Math::box3 &b )                 const;
	TestResult isInside( const Math::dvec2 &p, double radius ) const;
	TestResult isInside( const Math::dvec2 &p )                const;
	TestResult isInside( const Math::dvec3 &p, double radius ) const;
	TestResult isInside( const Math::dvec3 &p )                const;


private:
	std::vector<Math::dplane> _planes;
};

}}
#endif