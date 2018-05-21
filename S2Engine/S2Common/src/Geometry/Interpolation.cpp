// Interpolation.cpp
//
#include "Interpolation.h"

namespace glm {

//-------------------------------------------------------------------------------------------------
double Interpolation::CatmullRom
(
	double t,
	const double &p0,
	const double &p1,
	const double &p2,
	const double &p3
)
{
	const double t2 = t * t;
	const double t3 = t2 * t;
	
	return 
		0.5 * ( ( 2.0 * p1 ) + 
		( -p0 + p2 ) * t +
		( 2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3 ) * t2 +
		(      -p0 + 3.0 * p1 - 3.0 * p2 + p3 ) * t3 );
}

//-------------------------------------------------------------------------------------------------
glm::dvec2 Interpolation::CatmullRom
(
	double t,
	const glm::dvec2 &p0,
	const glm::dvec2 &p1,
	const glm::dvec2 &p2,
	const glm::dvec2 &p3
)
{
	return glm::dvec2(
		CatmullRom( t, p0.x, p1.x, p2.x, p3.x ),
		CatmullRom( t, p0.y, p1.y, p2.y, p3.y ) );
}

//-------------------------------------------------------------------------------------------------
glm::dvec3 Interpolation::CatmullRom
(
	double t,
	const glm::dvec3 &p0,
	const glm::dvec3 &p1,
	const glm::dvec3 &p2,
	const glm::dvec3 &p3
)
{
	return glm::dvec3(
		CatmullRom( t, p0.x, p1.x, p2.x, p3.x ),
		CatmullRom( t, p0.y, p1.y, p2.y, p3.y ),
		CatmullRom( t, p0.z, p1.z, p2.z, p3.z ) );
}

//-------------------------------------------------------------------------------------------------
/// Interpolate a set of points at given level
/** Returns a vector of (level+1) * pntIn.size() points ( if pntIn.size() > 2 ). */
void Interpolation::CatmullRom
(
	const int level,
	const std::vector<glm::dvec3> &pntIn,
	std::vector<glm::dvec3> &pntOut
)
{
	size_t np= pntIn.size();
	if( np <= 1 || np == 2 )
	{
		pntOut= pntIn;
		return;
	}

	size_t npmo= np-1; // np minus one
	
	const size_t k= level+1;

	pntOut.resize( k * npmo );

	const glm::dvec3 prePnt= pntIn[0]*2. - pntIn[1];
	const glm::dvec3 postPnt= pntIn[npmo]*2. - pntIn[npmo-1];

	for( size_t i1=0; i1 < npmo; ++i1 )
	{
		size_t i0,i2,i3;
		i0= i1-1; i2= i1+1; i3= i2+1;
		const glm::dvec3& p0= i1 > 0 ? pntIn[i0] : prePnt;
		const glm::dvec3& p3= i3 < np ? pntIn[i3] : postPnt;
		for( size_t j=0; j < k; ++j )
		{
			double t= double(j)/k;
			pntOut[i1*k+j]= CatmullRom( t, p0, pntIn[i1], pntIn[i2], p3 );
		}
	}
	pntOut[k * npmo - 1]= pntIn.back();
}

}