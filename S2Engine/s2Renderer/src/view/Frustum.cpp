// Frustum.cpp
//
#include "Frustum.h"

#include <iostream>

using namespace s2;
using namespace Renderer;

// ------------------------------------------------------------------------------------------------
Frustum::Frustum()
: _planes( 6 )
{}

// ------------------------------------------------------------------------------------------------
void Frustum::update( const Math::dmat4 &modelView, const Math::dmat4 &projection )
{
	const Math::dmat4 clipMatrix = projection * modelView;

	const Math::dvec4 col0 = Math::row( clipMatrix, 0 );
	const Math::dvec4 col1 = Math::row( clipMatrix, 1 );
	const Math::dvec4 col2 = Math::row( clipMatrix, 2 );
	const Math::dvec4 col3 = Math::row( clipMatrix, 3 );

	_planes[0] = Math::normalize( Math::dplane( col3 - col0 ) );
	_planes[1] = Math::normalize( Math::dplane( col3 + col0 ) );
	_planes[2] = Math::normalize( Math::dplane( col3 - col1 ) );
	_planes[3] = Math::normalize( Math::dplane( col3 + col1 ) );
	_planes[4] = Math::normalize( Math::dplane( col3 - col2 ) );
	_planes[5] = Math::normalize( Math::dplane( col3 + col2 ) );
}

// ------------------------------------------------------------------------------------------------
Frustum::TestResult Frustum::isInside( const Math::box3 &b ) const
{
	//TestResult result = FrustumInside;

	//const Math::dvec3 minP(b.maxPoint());
	//const Math::dvec3 maxP(b.minPoint());


	//for(int i=0;i<6;++i)
	//{
	//	Math::dvec3 p(minP);
	//	{
	//		if (_planes[i].normal().x >= 0) p.x = maxP.x;
	//		if (_planes[i].normal().y >= 0) p.y = maxP.y;
	//		if (_planes[i].normal().z >= 0) p.z = maxP.z;
	//	}

	//	if( _planes[i].distanceFrom(p) < 0 )
	//	{
	//		std::cout << i << std::endl;
	//		return FrustumOutside;
	//	}
	//	else
	//	{
	//		Math::dvec3 n(maxP);
	//		{
	//			if (_planes[i].normal().x >= 0) p.x = minP.x;
	//			if (_planes[i].normal().y >= 0) p.y = minP.y;
	//			if (_planes[i].normal().z >= 0) p.z = minP.z;
	//		}
	//		if( _planes[i].distanceFrom(n) < 0 )
	//			result = FrustumIntersect;
	//	}
	//}

	//return result;
	return isInside( b.center(), b.diag()*.5 );
}

// ------------------------------------------------------------------------------------------------
Frustum::TestResult Frustum::isInside( const Math::dvec2 &p, double radius ) const
{
	return FrustumInside;
}

// ------------------------------------------------------------------------------------------------
Frustum::TestResult Frustum::isInside( const Math::dvec3 &p, double radius ) const
{
	TestResult result = FrustumInside;

	for( int i=0; i < 6; ++i )
	{
		const double dist = _planes[i].distanceFrom( p );
		if( dist < -radius )
		{
			return FrustumOutside;
		}
		else if( dist < radius )
		{
			result = FrustumIntersect;
		}
	}

	return result;
}

// ------------------------------------------------------------------------------------------------
Frustum::TestResult Frustum::isInside( const Math::dvec3 &p ) const
{
	for( int i=0; i < 6; ++i )
	{
		const double dist = _planes[i].distanceFrom( p );
		//if(_planes[i].isPointBehind(p))
		if( dist < 0 )
			return FrustumOutside;
	}

	return FrustumInside;
}

// ------------------------------------------------------------------------------------------------
Frustum::TestResult Frustum::isInside( const Math::dvec2 &p ) const
{
	return FrustumInside;
}