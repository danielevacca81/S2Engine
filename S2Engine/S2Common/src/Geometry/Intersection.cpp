// Intersection.cpp
//
#include "Intersection.h"
#include "Math\Box.h"


namespace glm {

//-------------------------------------------------------------------------------------------------
bool lineIntersection( const Math::dvec2 &a1, 
					   const Math::dvec2 &a2,
					   const Math::dvec2 &b1,
					   const Math::dvec2 &b2,
					   Math::dvec2 *out )
{
	const Math::dvec2 a( a2-a1 );
	const Math::dvec2 b( b2-b1 );
	const Math::dvec2 perpA( -a.y, a.x );
	const Math::dvec2 perpB( -b.y, b.x );

	const double d = Math::dot(perpA, b);
	if( Math::abs(d) < 1e-8 )
		return false;

	Math::dvec2 c( b2-a2 );
	double aa = Math::dot( perpA, c );
	double bb = Math::dot( perpB, c );

	if( d < 0 )
    {
        if( aa > 0 )	return false;
        if( bb > 0 )	return false;
        if( aa < d )	return false;
        if( bb < d )	return false;
    }
    else
    {
        if( aa < 0 )	return false;
        if( bb < 0 )	return false;
        if( aa > d )	return false;
        if( bb > d )	return false;
    }

	if( out )
	{
        const double t = 1.0 - (aa / d);
		
		*out = b1 + (b * t);
	}
    return true;

	//const double A1 = p1.y-p0.y;
	//const double B1 = p0.x-p1.x;
	//const double C1 = A1*p0.x+ B1*p0.y;

	//const double A2 = p3.y-p2.y;
	//const double B2 = p2.x-p3.x;
	//const double C2 = A2*p2.x+ B2*p2.y;

	//const double d  = A1*B2 - A2*B1;

	//if( fabs(d) < 1e-8 )
	//	return false;

	//out.x = (B2*C1 - B1*C2) / d;
	//out.y = (A1*C2 - A2*C1) / d;

	//const double minX = std::min( p0.x,p1.x );
	//const double maxX = std::max( p0.x,p1.x );
	//const double minY = std::min( p0.y,p1.y );
	//const double maxY = std::max( p0.y,p1.y );

	//if( out.x >= minX && out.x <= maxX &&
	//	out.y >= minY && out.y <= maxY )
	//	return true;
	//
	//return false;
}

//-------------------------------------------------------------------------------------------------
bool lineBoxIntersection( 
	const Math::dvec3 &p0,
	const Math::dvec3 &p1, 
	const Math::box3 &b )
{
	// test p0-p1 against box edges
	bool intersect = false;

	// 3D implementation: TODO!
	const Math::dvec2 p0_( p0.x,p0.y );
	const Math::dvec2 p1_( p1.x,p1.y );

	intersect |= Math::lineIntersection( p0_,p1_, Math::dvec2(b.minPoint().x,b.minPoint().y), Math::dvec2(b.minPoint().x,b.maxPoint().y) ); // left
	intersect |= Math::lineIntersection( p0_,p1_, Math::dvec2(b.minPoint().x,b.maxPoint().y), Math::dvec2(b.maxPoint().x,b.maxPoint().y) ); // top
	intersect |= Math::lineIntersection( p0_,p1_, Math::dvec2(b.maxPoint().x,b.maxPoint().y), Math::dvec2(b.maxPoint().x,b.minPoint().y) ); // right
	intersect |= Math::lineIntersection( p0_,p1_, Math::dvec2(b.maxPoint().x,b.minPoint().y), Math::dvec2(b.minPoint().x,b.minPoint().y) ); // bottom

	return intersect;
}

//-------------------------------------------------------------------------------------------------
// Cross product for 2D vectors
double cross(const Math::dvec2& a, const Math::dvec2& b)
{
	return a.x * b.y - a.y * b.x;
}

//-------------------------------------------------------------------------------------------------
// Check if two points p1, p1 are on the same side of line a-b (in 2D)
bool pointsOnSameSideOfLine
(
	const Math::dvec2 &p1,
	const Math::dvec2 &p2, 
	const Math::dvec2 &a,
	const Math::dvec2 &b
)
{
    double cp1 = cross( b-a, p1-a );
    double cp2 = cross( b-a, p2-a );
    if( ( cp1 >= 0 && cp2 >= 0 ) || ( cp1 < 0 && cp2 < 0 ) )
	{
		return true;
	}
    return false;
}

//-------------------------------------------------------------------------------------------------
// Check a point belongs to triangle in 2D
bool pointBelongsToTriangle
(
	const Math::dvec2 &p,
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &a3
)
{
    if( pointsOnSameSideOfLine( p, a1, a2, a3 ) && 
		pointsOnSameSideOfLine( p, a2, a3, a1 ) &&
		pointsOnSameSideOfLine( p, a3, a1, a2 ) )
	{
		return true;
	}
    return false;
}

//-------------------------------------------------------------------------------------------------
// Check 2 triangles intersects in 2D
bool triangles2DIntersection
(
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &a3,
	const Math::dvec2 &b1, 
	const Math::dvec2 &b2,
	const Math::dvec2 &b3
)
{
	// if a point of triangle 'a' belongs to triangle 'b' -> intersection
	if( pointBelongsToTriangle( a1, b1, b2, b3 ) ||
		pointBelongsToTriangle( a2, b1, b2, b3 ) ||
		pointBelongsToTriangle( a3, b1, b2, b3 ) )
		return true;
	
	// if a point of triangle 'b' belongs to triangle 'a' -> intersection
	if( pointBelongsToTriangle( b1, a1, a2, a3 ) ||
		pointBelongsToTriangle( b2, a1, a2, a3 ) ||
		pointBelongsToTriangle( b3, a1, a2, a3 ) )
		return true;

	// if a side of triangle 'a' intersect a side of triangle 'b' -> intersection
	if( // a12 <-> b sides
		lineIntersection( a1, a2, b1, b2 ) ||
		lineIntersection( a1, a2, b2, b3 ) ||
		lineIntersection( a1, a2, b3, b1 ) ||
		// a23 <-> b sides
		lineIntersection( a2, a3, b1, b2 ) ||
		lineIntersection( a2, a3, b2, b3 ) ||
		lineIntersection( a2, a3, b3, b1 ) ||
		// a31 <-> c sides
		lineIntersection( a3, a1, b1, b2 ) ||
		lineIntersection( a3, a1, b2, b3 ) ||
		lineIntersection( a3, a1, b3, b1 ) )
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------
// Check if a triangle intersects a box in 2D (even if box3 is 3D)
bool triangleBox2DIntersection
(
	const Math::dvec2 &a1, 
	const Math::dvec2 &a2,
	const Math::dvec2 &a3,
	const Math::box3  &b
)
{
	// box vertices
	// 4 __ 3
	//  |  |  
	//  |__|
	// 1    2
 	const Math::dvec2 b1( b.minPoint().x, b.minPoint().y );
	const Math::dvec2 b2( b.maxPoint().x, b.minPoint().y );
	const Math::dvec2 b3( b.maxPoint().x, b.maxPoint().y );
	const Math::dvec2 b4( b.minPoint().x, b.maxPoint().y );

	// check against box triangles
	// 4   3
	//   /|  
	//  /_|
	// 1   2
 	if( triangles2DIntersection( a1, a2, a3, b1, b2, b3 ) )
		return true;
	
	// 4 __ 3
	//  | /
	//  |/
	// 1    2
	if( triangles2DIntersection( a1, a2, a3, b1, b3, b4 ) )
		return true;

	return false;
}

}