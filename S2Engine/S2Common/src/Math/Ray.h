// Ray.h
//
#ifndef RAY_ONCE
#define RAY_ONCE

// A point on ray is P = O + Dt

#include "Math.h"
#include "Plane.h"

namespace glm {
namespace detail {

template < typename T, precision P>
class tray
{
	enum ctor{null};

public:
	tvec3<T,P>  o;
	tvec3<T,P>  d;

public:
	// -----------------------------------------------------------------------------------------------
	GLM_FUNC_DECL tray()					{}
	GLM_FUNC_DECL tray( const tvec3<T,P> &origin, const tvec3<T,P> &direction )	: o(origin),d(direction) {}

	GLM_FUNC_DECL tvec3<T,P> pointAt( const T &t ) const { return o + d*t; }

	GLM_FUNC_DECL bool intersects( const tplane<T,P> &plane, T &t ) const
	{
		const T denom = dot(d,plane.n);

		if( Math::abs(denom) < 1e-5 )
			return false;

		t = - (dot(o,plane.n)+plane.d) / denom;
		
		return t >= 0;
	}

	// @ tbd
	//GLM_FUNC_DECL bool intersects( const tvec3<T,P> &center, const T &radius) const;
	//GLM_FUNC_DECL bool intersects( const tvec3<T,P> &v0, const tvec3<T,P> &v1, const tvec3<T,P> &v2 ) const;
};

} // end namespace detail

typedef detail::tray<float, defaultp>  fray;
typedef detail::tray<double, defaultp> dray;

}
#endif
