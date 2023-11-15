// Math.h
//
#ifndef MATH_MATH_H
#define MATH_MATH_H

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/random.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/type_precision.hpp"
#include "glm/gtc/epsilon.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/spline.hpp"
#include "glm/gtx/matrix_decompose.hpp"

//#include "glm/gtx/color_space.hpp"
//#include "glm/gtx/closest_point.hpp"
//#include "glm/gtx/gradient_paint.hpp"
//#include "glm/gtx/io.hpp"
//#include "glm/gtx/polar_coordinates.hpp"

//namespace s2 {
namespace Math = glm;

/* do not include any glm custom class/function into s2 or s2::Math namespace*/

//}

namespace glm {
	// -----------------------------------------------------------------------------------------------
	template < typename T, precision P >
	GLM_FUNC_QUALIFIER typename tvec2<T, P>::value_type
	angleBetween( const tvec2<T, P>& d, const tvec2<T, P>& u )
	{
		typename tvec2<T, P>::value_type angle = atan2( u.y, u.x ) - atan2( d.y, d.x );
		while( angle > pi<double>() ) angle -= pi<double>() * 2.0;
		while( angle <= -pi<double>() ) angle += pi<double>() * 2.0;
		return angle;
	}

	// -----------------------------------------------------------------------------------------------
	template < typename T, precision P >
	GLM_FUNC_QUALIFIER typename tvec3<T, P>::value_type
	angleBetween( const tvec3<T, P>& d, const tvec3<T, P>& u )
	{
		typename tvec3<T, P>::value_type angle = atan2( u.y, u.x ) - atan2( d.y, d.x );
		while( angle > pi<double>() ) angle -= pi<double>() * 2.0;
		while( angle <= -pi<double>() ) angle += pi<double>() * 2.0;
		return angle;
	}

	// -----------------------------------------------------------------------------------------------
	template <typename genType>
	GLM_FUNC_DECL genType
	round( genType const& value, int precision )
	{
		genType g = (genType) 1;
		if( precision > 0 )
			g = pow<genType>( (genType) 10, precision );

		return round( value * g ) / g;
	}

	// -----------------------------------------------------------------------------------------------
	inline int log2( unsigned int x )
	{
		int y = 0;
		while( x >>= 1 ) ++y;
		return y;
	}

	// ------------------------------------------------------------------------------------------------
	inline bool isPowerOfTwo( unsigned int x )
	{
		return ( ( x & ( x - 1 ) ) == 0 );
	}

	// ------------------------------------------------------------------------------------------------
	inline unsigned int nextPowOfTwo( unsigned int x )
	{
		x--;
		x |= x >> 1;  // handle  2 bit numbers
		x |= x >> 2;  // handle  4 bit numbers
		x |= x >> 4;  // handle  8 bit numbers
		x |= x >> 8;  // handle 16 bit numbers
		x |= x >> 16; // handle 32 bit numbers
	#if 0	
		// @TODO: Enable for 64bit compilers
		x |= x >> 32; // handle 64 bit numbers
	#endif

		x++;

		return x;
	}


	// ------------------------------------------------------------------------------------------------
	/// Returns the component-wise comparison of |x - y| / min(|x|,|y|) <= epsilon.
	/// True if this expression is satisfied.
	template<typename genType>
	GLM_FUNC_DECL bool epsilonEqualRelative( genType const& x, genType const& y, genType const& epsilon );

	template<>
	GLM_FUNC_QUALIFIER bool epsilonEqualRelative
	(
		float const& x,
		float const& y,
		float const& epsilon
	)
	{
		return	( x == y ) ||
			abs( x - y ) / min( abs( x ), abs( y ) ) <= epsilon;
	}

	template<>
	GLM_FUNC_QUALIFIER bool epsilonEqualRelative
	(
		double const& y,
		double const& x,
		double const& epsilon
	)
	{
		return	( x == y ) ||
			abs( x - y ) / min( abs( x ), abs( y ) ) <= epsilon;
	}

	// ------------------------------------------------------------------------------------------------
	/// Returns the component-wise comparison of |x - y| / min(|x|,|y|) <= epsilon.
	/// True if this expression is satisfied.
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, bool, Q> epsilonEqualRelative( vec<L, T, Q> const& x, vec<L, T, Q> const& y, T const& epsilon );

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, bool, Q> epsilonEqualRelative( vec<L, T, Q> const& x, vec<L, T, Q> const& y, T const& epsilon )
	{
		return	equal( x, y ) ||
			lessThanEqual( abs( x - y ) / min( abs( x ), abs( y ) ), vec<L, T, Q>( epsilon ) );
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, bool, Q> epsilonEqualRelative( vec<L, T, Q> const& x, vec<L, T, Q> const& y, vec<L, T, Q> const& epsilon )
	{
		return	equal( x, y ) ||
			lessThanEqual( abs( x - y ) / min( abs( x ), abs( y ) ), epsilon );
	}

}

#endif