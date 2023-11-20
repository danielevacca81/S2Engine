// Frustum.cpp
//
#include "Frustum.h"

using namespace Scene;

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
Frustum::TestResult Frustum::isInside( const Math::dbox3 &b ) const
{

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