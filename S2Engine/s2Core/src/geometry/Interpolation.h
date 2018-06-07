// Interpolation.h
//
#ifndef INTERPOLATION_ONCE
#define INTERPOLATION_ONCE

#include "s2Core_API.h"

#include "Math/Math.h"

#include <vector>

namespace glm {

//-------------------------------------------------------------------------------------------------
/// Interpolation class.
/**
	This class contains some static functions to perform interpolation of data and points.
*/
class S2CORE_API Interpolation
{
public:
	/// Interpolate between p1 (t==0) and p2 (t==1) values given four control points
	/** (see for example http://www.mvps.org/directx/articles/catmull/ and http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmulll.E2.80.93Rom_spline) */
	static double CatmullRom
	(
		double t,
		const double &p0,
		const double &p1,
		const double &p2,
		const double &p3
	);

	/// Interpolate between p1 (t==0) and p2 (t==1) values given four control points
	/** (see for example http://www.mvps.org/directx/articles/catmull/ and http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmulll.E2.80.93Rom_spline) */
	static glm::dvec2 CatmullRom
	(
		double t,
		const glm::dvec2 &p0,
		const glm::dvec2 &p1,
		const glm::dvec2 &p2,
		const glm::dvec2 &p3
	);

	/// Interpolate between p1 (t==0) and p2 (t==1) values given four control points
	/** (see for example http://www.mvps.org/directx/articles/catmulll/ and http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmulll.E2.80.93Rom_spline) */
	static glm::dvec3 CatmullRom
	(
		double t,
		const glm::dvec3 &p0,
		const glm::dvec3 &p1,
		const glm::dvec3 &p2,
		const glm::dvec3 &p3
	);

	/// Interpolate a set of points at given level
	/** Returns a vector of (level+1) * pntIn.size() points */
	static void CatmullRom
	(
		const int level,
		const std::vector<glm::dvec3> &pntIn,
		std::vector<glm::dvec3> &pntOut
	);
};

}
#endif