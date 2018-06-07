// Plane.h
//
#ifndef PLANE_ONCE
#define PLANE_ONCE

// Plane equation:
//
// (Ax + By + Cz + D = 0)
//
// yelds
// 
// D = -(Ax + By + Cz) 
// 
// where n(A,B,C) is the plane normal vector and p(x,y,z) is a point on the plane 
// 
// -D is the scalar product of n and p

#include "Math.h"

namespace glm {
namespace detail {

template < typename T, precision P>
class tplane
{
public:
	tvec3<T,P>  n;
	T           d;

public:
	// -----------------------------------------------------------------------------------------------
	GLM_FUNC_DECL tplane()					{}
	GLM_FUNC_DECL tplane( const tvec3<T,P> &normal, T distance )	: n(normal),d(distance) {}
	GLM_FUNC_DECL tplane( const tvec4<T,P> &coeff  )	{ n = tvec3<T,P>(coeff.x,coeff.y,coeff.z); d = coeff.w; }

	GLM_FUNC_DECL bool isPointFront ( const tvec3<T,P> &p ) const { return (distanceFrom(p) > 0.0) ? true: false; }
	GLM_FUNC_DECL bool isPointBehind( const tvec3<T,P> &p ) const { return !(isPointFront(p)); }
	GLM_FUNC_DECL T    distanceFrom ( const tvec3<T,P> &p ) const { return dot(n,p) + d; }
};

} // end namespace detail


// -----------------------------------------------------------------------------------------------
template <typename T, precision P>
inline GLM_FUNC_DECL detail::tplane<T, P> normalize(
	detail::tplane<T, P> const & p)
{
	const T ilen = 1.0/Math::length(p.n);

	return detail::tplane<T,P>(p.n * ilen, p.d * ilen);
}

typedef detail::tplane<float, defaultp>  fplane;
typedef detail::tplane<double, defaultp> dplane;


}
#endif
